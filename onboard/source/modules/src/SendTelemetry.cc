#include "SendTelemetry.hh"
#include "CommunicationCodes.hh"

using namespace anlnext;

namespace gramsballoon::pgrams {

SendTelemetry::SendTelemetry() {
  errorManager_ = std::make_shared<ErrorManager>();
  binaryFilenameBase_ = "Telemetry";
  mhadcMapping_ = std::make_shared<MHADCMapping>();
}

SendTelemetry::~SendTelemetry() = default;

ANLStatus SendTelemetry::mod_define() {
  define_parameter("save_telemetry", &mod_class::saveTelemetry_);
  define_parameter("binary_filename_base", &mod_class::binaryFilenameBase_);
  define_parameter("num_telem_per_file", &mod_class::numTelemPerFile_);
  define_parameter("minimum_send_time", &mod_class::minimumSendTime_);
  set_parameter_description("Minimum time interval between telemetry sending");
  set_parameter_unit(1.0, "ms");
  define_parameter("topic", &mod_class::pubTopic_);
  set_parameter_description("MQTT topic for telemetry publishing via Iridium");
  define_parameter("starlink_topic", &mod_class::starlinkTopic_);
  set_parameter_description("MQTT topic for telemetry publishing via Starlink");
  define_parameter("qos", &mod_class::qos_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}

ANLStatus SendTelemetry::mod_initialize() {

  const std::string receive_command_md = "ReceiveCommand";
  if (exist_module(receive_command_md)) {
    get_module_NC(receive_command_md, &receiveCommand_);
  }

  const std::string run_id_manager_md = "RunIDManager";
  if (exist_module(run_id_manager_md)) {
    get_module_NC(run_id_manager_md, &runIDManager_);
  }

  // communication
  if (exist_module("TelemMosquittoManager")) {
    get_module_NC("TelemMosquittoManager", &mosquittoManager_);
  }
  else {
    std::cerr << "Error in SendTelemetry::mod_initialize: MosquittoManager module not found." << std::endl;
    return AS_ERROR;
  }
  mosq_ = mosquittoManager_->getMosquittoIO();
  if (!mosq_) {
    std::cerr << "MosquittoIO is nullptr" << std::endl;
    return AS_ERROR;
  }
  telemdef_ = std::make_shared<HubHKTelemetry>(true);
  mhadcMapping_->setHKTelemetry(telemdef_);
  if (saveTelemetry_) {
    telemetrySaver_ = std::make_shared<CommunicationSaver<std::string>>();
  }
  if (telemetrySaver_) {
    telemetrySaver_->setBinaryFilenameBase(binaryFilenameBase_);
    if (runIDManager_) {
      telemetrySaver_->setRunID(runIDManager_->RunID());
      telemetrySaver_->setTimeStampStr(runIDManager_->TimeStampStr());
    }
  }
  getHKModules();
  return AS_OK;
}

ANLStatus SendTelemetry::mod_analyze() {
  if (chatter_ > 0) std::cout << "SendTelemetry::mod_analyze" << std::endl;
  telemetryStr_.clear();
  if (mosq_ == nullptr) {
    std::cout << module_id() << ": mosq_ is nullptr" << std::endl;
    return AS_OK;
  }
  if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastSendTime_).count() < minimumSendTime_) {
    return AS_OK;
  }
  lastSendTime_ = std::chrono::steady_clock::now();
  telemdef_->setCurrentTime();
  telemdef_->getContentsNC()->setCode(::pgrams::communication::to_telem_u16(::pgrams::communication::TelemetryCodes::HUB_Telemetry_Normal));
  telemdef_->setIndex(telemIndex_);
  telemdef_->setRunID(runIDManager_->RunID());
  setHKTelemetry();
  telemIndex_++;
  telemdef_->update();
  telemdef_->construct(telemetryStr_);
  if (chatter_ > 2) {
    std::cout << module_id() << ":telelemetry is " << telemetryStr_ << std::endl;
  }
  int status;
  if (mosquittoManager_->getLinkType() == CommunicationLinkType::STARLINK) {
    status = mosq_->Publish(telemetryStr_, starlinkTopic_, qos_);
  }
  else if (mosquittoManager_->getLinkType() == CommunicationLinkType::IRIDIUM) {
    status = mosq_->Publish(telemetryStr_, pubTopic_, qos_);
  }
  else {
    std::cerr << "SendTelemetry::mod_analyze: Unknown CommunicationLinkType" << std::endl;
    return AS_ERROR;
  }
  const bool failed = (status != mosq_err_t::MOSQ_ERR_SUCCESS);
  if (failed) {
    std::cerr << "Error in SendTelemetry::mod_analyze: Publishing MQTT failed.Error Message: " << mosqpp::strerror(status) << std::endl;
    errorManager_->setError(ErrorType::MQTT_COM_ERROR);
  }
  if (saveTelemetry_ && telemetrySaver_) {
    telemetrySaver_->writeCommandToFile(failed, telemetryStr_);
  }

  if (chatter_ > 3) {
    std::cout << (int)telemetryStr_.size() << std::endl;
    for (int i = 0; i < (int)telemetryStr_.size(); i++) {
      std::cout << i << " " << static_cast<int>(telemetryStr_[i]) << std::endl;
    }
  }

  return AS_OK;
}

ANLStatus SendTelemetry::mod_finalize() {
  return AS_OK;
}

void SendTelemetry::setLastComIndex(Subsystem subsystem, uint32_t v) {
  std::cout << "subsystem: " << static_cast<int>(subsystem) << "last Com index: " << v << std::endl;
  if (subsystem == Subsystem::ORC) {
    telemdef_->setLastCommandIndexOrc(v);
  }
  else if (subsystem == Subsystem::HUB) {
    telemdef_->setLastCommandIndexHub(v);
  }
  else if (subsystem == Subsystem::QM) {
    telemdef_->setLastCommandIndexQM(v);
  }
  else if (subsystem == Subsystem::TOF) {
    telemdef_->setLastCommandIndexTOF(v);
  }
  else if (subsystem == Subsystem::COL) {
    telemdef_->setLastCommandIndexTPC(v);
  }
  else {
    std::cerr << "SendTelemetry::setLastComIndex: Unknown subsystem" << std::endl;
  }
}
void SendTelemetry::setLastComCode(Subsystem subsystem, uint16_t v) {
  std::cout << "subsystem: " << static_cast<int>(subsystem) << "last Com code: " << v << std::endl;
  if (subsystem == Subsystem::ORC) {
    telemdef_->setLastCommandCodeOrc(v);
  }
  else if (subsystem == Subsystem::HUB) {
    telemdef_->setLastCommandCodeHub(v);
  }
  else if (subsystem == Subsystem::QM) {
    telemdef_->setLastCommandCodeQM(v);
  }
  else if (subsystem == Subsystem::TOF) {
    telemdef_->setLastCommandCodeTOF(v);
  }
  else if (subsystem == Subsystem::COL) {
    telemdef_->setLastCommandCodeTPC(v);
  }
  else {
    std::cerr << "SendTelemetry::setLastComCode: Unknown subsystem" << std::endl;
  }
}

void SendTelemetry::setCommandRejectedIndex(Subsystem subsystem, uint32_t v) {
  if (subsystem == Subsystem::ORC) {
    telemdef_->setCommandRejectedIndexOrc(v);
  }
  else if (subsystem == Subsystem::HUB) {
    telemdef_->setCommandRejectedIndexHub(v);
  }
  else if (subsystem == Subsystem::QM) {
    telemdef_->setCommandRejectedIndexQM(v);
  }
  else if (subsystem == Subsystem::TOF) {
    telemdef_->setCommandRejectedIndexTOF(v);
  }
  else if (subsystem == Subsystem::COL) {
    telemdef_->setCommandRejectedIndexTPC(v);
  }
  else {
    std::cerr << "SendTelemetry::setCommandRejectedIndex: Unknown subsystem" << std::endl;
  }
}

void SendTelemetry::getHKModules() {
  if (exist_module("GetMHADCData")) {
    get_module("GetMHADCData", &getMhadcData_);
  }
  else {
    std::cerr << "Error in SendTelemetry::getHKModules: GetMHADCData module not found." << std::endl;
  }
#ifdef USE_SYSTEM_MODULES
  if (exist_module("GetComputerStatus")) {
    get_module("GetComputerStatus", &getComputerStatus_);
  }
  else {
    std::cerr << "Error in SendTelemetry::getHKModules: GetComputerStatus module not found." << std::endl;
  }
#else
  std::cerr << "SendTelemetry::getHKModules: GetComputerStatus is disabled." << std::endl;
#endif
}

void SendTelemetry::setHKTelemetry() {
  if (!telemdef_) {
    telemdef_ = std::make_shared<HubHKTelemetry>(true);
    mhadcMapping_->setHKTelemetry(telemdef_);
  }
  for (int i = 0; i < static_cast<int>(HubHKTelemetry::NUM_ERROR_FLAGS); i++) {
    telemdef_->setHubComputerErrorFlags(i, errorManager_->ErrorCode(i));
  }
  {
    if (getMhadcData_ == nullptr) {
      std::cerr << "SendTelemetry::setHKTelemetry: getMhadcData_ is nullptr" << std::endl;
    }
    else {
      const int num_ch = getMhadcData_->NumCH();
      const auto adcs = getMhadcData_->AdcData();
      for (int i = 0; i < num_ch; i++) {
        mhadcMapping_->setValue(i, adcs[i]);
      }
    }
  }
  if (receiveCommand_) {
    const auto v = receiveCommand_->CommandRejectCount();
    telemdef_->setCommandRejectedIndexHub(v);
  }
  
#ifdef USE_SYSTEM_MODULES
  {
    if (getComputerStatus_ == nullptr) {
      std::cerr << "SendTelemetry::setHKTelemetry: getComputerStatus_ is nullptr" << std::endl;
    }
    else {
      telemdef_->setCpuTemperature(getComputerStatus_->CPUTemperatureADC());
      constexpr uint64_t toMB = (1 << 20);
      constexpr uint64_t kBtoMB = (1 << 10);
      telemdef_->setStorageSize(getComputerStatus_->CapacityFree() / toMB);
      telemdef_->setRamUsage(getComputerStatus_->MemoryUsage() / kBtoMB);
    }
    if (chatter_ > 1) {
      telemdef_->print(std::cout);
    }
  }
#endif
}

} // namespace gramsballoon::pgrams
