#include "ReceiveStatusFromDAQComputer.hh"
#include <iomanip>
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus ReceiveStatusFromDAQComputer::mod_define() {
  set_module_description("This module is responsible for receiving status from DAQ computers. SocketCommunicationManager is required for communication.");
  define_parameter("SocketCommunicationManager_name", &mod_class::socketCommunicationManagerName_);
  set_parameter_description("Name of SocketCommunicationManager");
  define_parameter("chatter", &mod_class::chatter_);
  define_parameter("dead_communication_time", &mod_class::deadCommunicationTime_);
  set_parameter_description("Time in milliseconds to consider the communication dead if no data is received. If 0 is specified, assumes no dead communication time.");
  set_parameter_unit(1.0, "ms");
  define_parameter("save_status", &mod_class::saveStatus_);
  set_parameter_description("Whether to save the received status to a binary file.");
  define_parameter("binary_filename_base", &mod_class::binaryFilenameBase_);
  set_parameter_description("Base name for the binary files to save the received status.");
  define_parameter("num_status_per_file", &mod_class::numStatusPerFile_);
  set_parameter_description("Number of status messages to save per binary file.");
  define_parameter("subsystem", &mod_class::subsystemInt_);
  return AS_OK;
}
ANLStatus ReceiveStatusFromDAQComputer::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  else {
    std::cerr << "Error in " << module_id() << "::mod_initialize: SendTelemetry not found." << std::endl;
  }
  if (exist_module(socketCommunicationManagerName_)) {
    get_module_NC(socketCommunicationManagerName_, &socketCommunicationManager_);
  }
  else {
    std::cerr << "Error in " << module_id() << "::mod_initialize: SocketCommunicationManager not found." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
  }
  subsystem_ = magic_enum::enum_cast<Subsystem>(subsystemInt_).value_or(Subsystem::UNKNOWN);
  if (subsystem_ == Subsystem::UNKNOWN) {
    std::cerr << "Error in " << module_id() << "::mod_initialize: Invalid subsystem value " << subsystemInt_ << ". Setting to UNKNOWN." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::OTHER_ERRORS);
    }
  }
  deadCommunicationTimeChrono_ = std::chrono::milliseconds(deadCommunicationTime_);
  if (saveStatus_) {
    statusSaver_ = std::make_shared<CommunicationSaver<std::vector<uint8_t>>>();
  }
  if (statusSaver_) {
    statusSaver_->setBinaryFilenameBase(binaryFilenameBase_);
    statusSaver_->setNumCommandPerFile(numStatusPerFile_);
    if (exist_module("RunIDManager")) {
      const RunIDManager *runIDManager = nullptr;
      get_module("RunIDManager", &runIDManager);
      statusSaver_->setRunID(runIDManager->RunID());
      statusSaver_->setTimeStampStr(runIDManager->TimeStampStr());
    }
  }
  return AS_OK;
}
ANLStatus ReceiveStatusFromDAQComputer::mod_analyze() {
  if (!socketCommunicationManager_) {
    std::cerr << module_id() << "::mod_analyze: SocketCommunicationManager is nullptr." << std::endl;
    return AS_OK;
  }
  auto sc = socketCommunicationManager_->getSocketCommunication();
  if (!sc) {
    std::cerr << module_id() << "::mod_analyze: SocketCommunication in the SocketCommunicationManager is nullptr." << std::endl;
    return AS_OK;
  }
  if (sc->isFailed()) {
    std::cerr << module_id() << "::mod_analyze(): Socket Communication is failed." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqComErrorType(subsystem_, true));
    }
    return AS_OK;
  }
  if (!sc->isConnected()) {
    if (chatter_ > 2)
      std::cerr << module_id() << "::mod_analyze(): Socket Communication is not connected." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqComErrorType(subsystem_, false));
    }
    return AS_OK;
  }
  const auto now = std::chrono::steady_clock::now();
  const auto result = socketCommunicationManager_->receive(bufTmp_);
  if (result > 0) {
    buffer_.insert(buffer_.end(), bufTmp_.begin(), bufTmp_.begin() + result);
    lastReceivedTime_ = now;
    if (saveStatus_) {
      if (statusSaver_) {
        statusSaver_->writeCommandToFile(false, bufTmp_);
      }
    }
  }
  else if (result != 0) {
    std::cout << "Error in " << module_id() << "::mod_analyze: receiving data failed. error code = " << errno << "(" << strerror(errno) << ")" << std::endl;
  }
  if (chatter_ > 1) {
    std::cout << "Received " << result << " bytes." << std::endl;
  }
  if ((deadCommunicationTime_ > 0) && (lastReceivedTime_ + deadCommunicationTimeChrono_ < now)) {
    if (chatter_ > 0) {
      std::cerr << module_id() << "::mod_analyze: Communication is dead. No data received for " << deadCommunicationTime_ << " ms." << std::endl;
    }
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqComErrorType(subsystem_, false));
    }
    lastReceivedTime_ = now;
  }
  return AS_OK;
}
ANLStatus ReceiveStatusFromDAQComputer::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams
