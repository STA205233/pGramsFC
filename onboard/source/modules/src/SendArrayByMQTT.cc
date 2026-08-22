#include "SendArrayByMQTT.hh"
#include "CommunicationCodes.hh"
#include "MosquittoIO.hh"
#include "MosquittoManager.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus SendArrayByMQTT::mod_define() {
  define_parameter("InterpretTelemetry_name", &mod_class::interpretTelemetryName_);
  set_parameter_description("InterpretTelemetry name");
  define_parameter("MosquittoManager_name", &mod_class::mosquittoManagerName_);
  set_parameter_description("MosquittoManager name.");
  define_parameter("topic", &mod_class::topic_);
  set_parameter_description("MQTT topic to publish.");
  define_parameter("qos", &mod_class::qos_);
  set_parameter_description("MQTT QoS level.");
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus SendArrayByMQTT::mod_initialize() {
  if (exist_module(interpretTelemetryName_)) {
    get_module(interpretTelemetryName_, &interpretTelemetry_);
  }
  else {
    std::cerr << interpretTelemetryName_ << " does not exist" << std::endl;
    return AS_QUIT_ALL_ERROR;
  }
  if (exist_module(mosquittoManagerName_)) {
    get_module_NC(mosquittoManagerName_, &mosquittoManager_);
  }
  else {
    std::cerr << mosquittoManagerName_ << " does not exist" << std::endl;
    return AS_QUIT_ALL_ERROR;
  }
  return AS_OK;
}
ANLStatus SendArrayByMQTT::mod_analyze() {
  using namespace ::pgrams::communication;
  if (!mosquittoManager_ || !interpretTelemetry_) {
    return AS_OK;
  }
  if (interpretTelemetry_->CurrentTelemetryType() != to_telem_u16(TelemetryCodes::HUB_Telemetry_Normal) || interpretTelemetry_->CurrentTelemetryType() != to_telem_u16(TelemetryCodes::HUB_Tof_Bias_full) || interpretTelemetry_->CurrentTelemetryType() != to_telem_u16(TelemetryCodes::HUB_Tof_Bias_summary)) {
    return AS_OK;
  }
  const auto telemetry = interpretTelemetry_->getTelemetry();
  const auto &contents = telemetry->getContents()->Arguments();
  const auto code = telemetry->getContents()->Code();
  data_.clear();
  dataStr_.clear();
  dataStr_ = "{\"code\":" + std::to_string(static_cast<int>(code)) + ",\"argv\":[";
  const size_t sz = contents.size();
  for (size_t i = 0; i < sz - 1; i++) {
    dataStr_ += std::to_string(contents[i]) + ",";
  }
  if (sz != 0)
    dataStr_ += std::to_string(contents[sz - 1]);
  dataStr_ += "]}";
  data_.assign(dataStr_.begin(), dataStr_.end());
  auto mosquitto_io = mosquittoManager_->getMosquittoIO();
  if (chatter_ > 3) {
    std::cout << "Sending message to MQTT: " << dataStr_ << std::endl;
  }
  const int result = mosquitto_io->Publish(data_, topic_, qos_);
  if (result != MOSQ_ERR_SUCCESS) {
    std::cerr << module_id() << "::mod_analyze: Failed to publish MQTT message: " << mosqpp::strerror(result) << std::endl;
    return AS_ERROR;
  }
  return AS_OK;
}
} // namespace gramsballoon::pgrams