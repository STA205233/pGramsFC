#include "InterpretTelemetry.hh"
#include "DateManager.hh"
#include "HubHKTelemetry.hh"
#include <sstream>
using namespace anlnext;

namespace gramsballoon::pgrams {

template <typename TelemType>
InterpretTelemetry<TelemType>::InterpretTelemetry() {
  errorManager_ = std::make_shared<ErrorManager>();
  binaryFilenameBase_ = "Telemetry";
  runIDFilename_ = "/Users/grams/settings/run_id/run_id.txt";
  receiverModuleName_ = "ReceiveTelemetry";
  telemetry_ = std::make_shared<TelemType>(true);
  telemetrySaver_ = std::make_shared<CommunicationSaver<std::string>>();
}

template <typename TelemType>
ANLStatus InterpretTelemetry<TelemType>::mod_define() {
  define_parameter("save_telemetry", &mod_class::saveTelemetry_);
  define_parameter("num_telem_per_file", &mod_class::numTelemPerFile_);
  define_parameter("run_ID_filename", &mod_class::runIDFilename_);
  define_parameter("binary_filename_base", &mod_class::binaryFilenameBase_);
  define_parameter("receiver_module_name", &mod_class::receiverModuleName_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}

template <typename TelemType>
ANLStatus InterpretTelemetry<TelemType>::mod_initialize() {
  if (exist_module(receiverModuleName_)) {
    get_module_NC(receiverModuleName_, &receiver_);
  }
#ifdef USE_HSQUICKLOOK
  const std::string pusher_module_name = "PushToMongoDB";
  if (exist_module(pusher_module_name)) {
    get_module_NC(pusher_module_name, &pusher_);
  }
#endif // USE_HSQUICKLOOK
  telemetrySaver_->setNumCommandPerFile(numTelemPerFile_);
  telemetrySaver_->setBinaryFilenameBase(binaryFilenameBase_);
  telemetrySaver_->setRunID(0); // dummy
  telemetrySaver_->setTimeStampStr(getTimeStr());
  return AS_OK;
}

template <typename TelemType>
ANLStatus InterpretTelemetry<TelemType>::mod_analyze() {
  if (receiver_ == nullptr) {
    std::cerr << module_name() << "::mod_analyze: Receiver is nullptr" << std::endl;
    return AS_ERROR;
  }
  if (!(receiver_->Valid())) {
    currentTelemetryType_ = 0;
    return AS_OK;
  }

  currentTelemetryType_ = 0;
  const auto &telemetry = receiver_->Telemetry();
  const bool status = interpret(telemetry);
  const bool failed = !status;
  if (failed) {
    std::cerr << module_name() << "::mod_analyze Failed to interpret telemetry..." << std::endl;
    telemetrySaver_->writeCommandToFile(failed, telemetry);
    return AS_OK;
  }
  if (saveTelemetry_) {
    telemetrySaver_->writeCommandToFile(failed, telemetry);
  }
  return AS_OK;
}

template <typename TelemType>
ANLStatus InterpretTelemetry<TelemType>::mod_finalize() {
  return AS_OK;
}

template <typename TelemType>
void InterpretTelemetry<TelemType>::updateRunIDFile() {
  const std::string time_stamp_str = getTimeStr();
  std::ofstream ofs(runIDFilename_, std::ios::app | std::ios::out);
  ofs << currentRunID_ << " " << time_stamp_str << "\n";
  ofs.flush();
  ofs.close();
}

template <typename TelemType>
bool InterpretTelemetry<TelemType>::interpret(const std::string &telemetryStr) {
  if (!telemetry_) return false;
  const bool result = telemetry_->parseJSON(telemetryStr);
  if constexpr (std::is_same<TelemType, HubHKTelemetry>::value) {
    if (result && currentRunID_ < 0) {
      currentRunID_ = telemetry_->RunID();
      updateRunIDFile();
    }
  }
  if (chatter_ > 0) {
    telemetry_->print(std::cout);
  }
  return result;
}

template InterpretTelemetry<BaseTelemetryDefinition>::InterpretTelemetry();
template InterpretTelemetry<HubHKTelemetry>::InterpretTelemetry();
template ANLStatus InterpretTelemetry<BaseTelemetryDefinition>::mod_define();
template ANLStatus InterpretTelemetry<HubHKTelemetry>::mod_define();
template ANLStatus InterpretTelemetry<BaseTelemetryDefinition>::mod_initialize();
template ANLStatus InterpretTelemetry<HubHKTelemetry>::mod_initialize();
template ANLStatus InterpretTelemetry<BaseTelemetryDefinition>::mod_analyze();
template ANLStatus InterpretTelemetry<HubHKTelemetry>::mod_analyze();
template ANLStatus InterpretTelemetry<BaseTelemetryDefinition>::mod_finalize();
template ANLStatus InterpretTelemetry<HubHKTelemetry>::mod_finalize();
template bool InterpretTelemetry<BaseTelemetryDefinition>::interpret(const std::string &);
template bool InterpretTelemetry<HubHKTelemetry>::interpret(const std::string &);
template void InterpretTelemetry<BaseTelemetryDefinition>::updateRunIDFile();
template void InterpretTelemetry<HubHKTelemetry>::updateRunIDFile();
} // namespace gramsballoon::pgrams
