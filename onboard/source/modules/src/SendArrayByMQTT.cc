#include "SendArrayByMQTT.hh"
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
  if (!mosquittoManager_ || !interpretTelemetry_) {
    return AS_OK;
  }
  if (interpretTelemetry_->CurrentTelemetryType() == 0) {
    return AS_OK;
  }
  const auto telemetry = interpretTelemetry_->getTelemetry();
  const auto &contents = telemetry->getContents()->Command();
  data_.assign(contents.begin(), contents.end());
  auto mosquitto_io = mosquittoManager_->getMosquittoIO();
  if (chatter_ > 3) {
    std::cout << "Sending message to MQTT: ";
    for (auto d: data_) {
      std::cout << static_cast<int>(d) << " ";
    }
    std::cout << std::endl;
  }
  const int result = mosquitto_io->Publish(data_, topic_, qos_);
  if (result != MOSQ_ERR_SUCCESS) {
    std::cerr << module_id() << "::mod_analyze: Failed to publish MQTT message: " << mosqpp::strerror(result) << std::endl;
    return AS_ERROR;
  }
  return AS_OK;
}
} // namespace gramsballoon::pgrams