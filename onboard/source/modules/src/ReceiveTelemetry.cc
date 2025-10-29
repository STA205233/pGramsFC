#include "ReceiveTelemetry.hh"
using namespace anlnext;

namespace gramsballoon::pgrams {
ANLStatus ReceiveTelemetry::mod_define() {
  define_parameter("topic", &mod_class::subTopic_);
  set_parameter_description("Topic to subscribe");
  define_parameter("qos", &mod_class::qos_);
  set_parameter_description("QoS");
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}

ANLStatus ReceiveTelemetry::mod_initialize() {
  const std::string mosq_md = "TelemMosquittoManager";
  if (exist_module(mosq_md)) {
    get_module_NC(mosq_md, &mosquittoManager_);
  }
  else {
    std::cerr << "Error in ReceiveTelemetry::mod_initialize: MosquittoManager module not found." << std::endl;
    return AS_ERROR;
  }
  mosq_ = mosquittoManager_->getMosquittoIO();
  if (!mosq_) {
    std::cerr << "MosquittoIO is nullptr" << std::endl;
    return AS_ERROR;
  }
  mosq_->Subscribe(subTopic_, qos_);
  telemetry_ = "";
  valid_ = false;
  return AS_OK;
}
ANLStatus ReceiveTelemetry::mod_analyze() {
  if (mosq_ == nullptr) {
    return AS_OK;
  }
  const auto &payload = mosq_->getPayload();
  if (payload.empty()) {
    if (chatter_ >= 1) {
      std::cout << "ReceiveTelemetry: No payload" << std::endl;
    }
    valid_ = false;
    return AS_OK;
  }
  const auto &packet = payload.front();
  if (packet->topic != subTopic_) {
    if (chatter_ >= 1) {
      std::cout << "ReceiveTelemetry: Received topic (" << payload.front()->topic << ") is different from subscribed topic (" << subTopic_ << ")" << std::endl;
    }
    valid_ = false;
    return AS_OK;
  }
  if (chatter_ >= 1) {
    std::cout << "ReceiveTelemetry Num_packet:" << payload.size() << std::endl;
  }
  if (chatter_ > 3) {
    for (size_t i = 0; i < payload[0]->payload.size(); i++) {
      std::cout << "payload[" << i << "]=" << std::hex << static_cast<unsigned int>(payload[0]->payload[i]) << std::dec << std::endl;
    }
  }
  setTelemetry(packet->payload);
  valid_ = true;
  mosq_->popPayloadFront();
  return AS_OK;
}

ANLStatus ReceiveTelemetry::mod_finalize() {
  return AS_OK;
}

} // namespace gramsballoon::pgrams
