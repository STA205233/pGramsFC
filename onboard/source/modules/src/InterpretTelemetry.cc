#include "InterpretTelemetry.hh"
#include "DateManager.hh"
#include "HubHKTelemetry.hh"
#include <sstream>
using namespace anlnext;

namespace gramsballoon::pgrams {

InterpretTelemetry::InterpretTelemetry() {
  errorManager_ = std::make_shared<ErrorManager>();
  binaryFilenameBase_ = "Telemetry";
  runIDFilename_ = "/Users/grams/settings/run_id/run_id.txt";
  receiverModuleName_ = "ReceiveTelemetry";
  telemetrySaver_ = std::make_shared<CommunicationSaver<std::string>>();
}

ANLStatus InterpretTelemetry::mod_define() {
  define_parameter("save_telemetry", &mod_class::saveTelemetry_);
  set_parameter_description("Switch for saving telemetry");
  define_parameter("num_telem_per_file", &mod_class::numTelemPerFile_);
  set_parameter_description("number of packet per a file. This parameter is valid only when save_telemetry is true");
  define_parameter("run_ID_filename", &mod_class::runIDFilename_);
  set_parameter_description("Filename of run id, This paramater is valid only when save_telemetry is true");
  define_parameter("binary_filename_base", &mod_class::binaryFilenameBase_);
  define_parameter("receiver_module_name", &mod_class::receiverModuleName_);
  define_parameter("telemetry_type", &mod_class::telemetryTypeStr_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}

ANLStatus InterpretTelemetry::mod_initialize() {
  if (exist_module(receiverModuleName_)) {
    get_module_NC(receiverModuleName_, &receiver_);
  }
  if (telemetryTypeStr_ == "HK") {
    telemetry_ = std::make_shared<HubHKTelemetry>(true);
  }
  else if (telemetryTypeStr_ == "Base") {
    telemetry_ = std::make_shared<BaseTelemetryDefinition>(true);
  }

  else {
    std::cerr << module_name() << "::mod_initialize: Unknown telemetry type " << telemetryTypeStr_ << std::endl;
    telemetry_ = std::make_shared<BaseTelemetryDefinition>(true);
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

ANLStatus InterpretTelemetry::mod_analyze() {
  if (receiver_ == nullptr) {
    std::cerr << module_name() << "::mod_analyze: Receiver is nullptr" << std::endl;
    return AS_ERROR;
  }
  if (!(receiver_->Valid())) {
    currentTelemetryType_ = 0;
    return AS_OK;
  }

  currentTelemetryType_ = 1;
  const auto &telemetry = receiver_->Telemetry();
  const bool status = interpret(telemetry);
  const bool failed = !status;
  if (failed) {
    std::cerr << module_name() << "::mod_analyze Failed to interpret telemetry..." << std::endl;
    telemetrySaver_->writeCommandToFile(failed, telemetry);
    currentTelemetryType_ = -1;
    return AS_OK;
  }
  if (saveTelemetry_) {
    telemetrySaver_->writeCommandToFile(failed, telemetry);
  }
  return AS_OK;
}

ANLStatus InterpretTelemetry::mod_finalize() {
  return AS_OK;
}

void InterpretTelemetry::updateRunIDFile() {
  const std::string time_stamp_str = getTimeStr();
  std::ofstream ofs(runIDFilename_, std::ios::app | std::ios::out);
  ofs << currentRunID_ << " " << time_stamp_str << "\n";
  ofs.flush();
  ofs.close();
}

bool InterpretTelemetry::interpret(const std::string &telemetryStr) {
  if (!telemetry_) return false;
  const bool result = telemetry_->parseJSON(telemetryStr);
  if (telemetryTypeStr_ == "HK") {
    if (result && currentRunID_ < 0) {
      currentRunID_ = telemetry_->RunID();
      updateRunIDFile();
    }
  }
  if (result && chatter_ > 0) {
    telemetry_->print(std::cout);
  }
  return result;
}

} // namespace gramsballoon::pgrams
