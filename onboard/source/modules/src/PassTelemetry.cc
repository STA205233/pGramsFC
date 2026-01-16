#include "PassTelemetry.hh"
#include "SendTelemetry.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus PassTelemetry::mod_define() {
  define_parameter("MosquittoManager_name", &mod_class::mosquittoManagerName_);
  set_parameter_description("Name of MosquittoManager");
  define_parameter("topic", &mod_class::topic_);
  set_parameter_description("Topic for iridium packets");
  define_parameter("starlink_topic", &mod_class::starlinkTopic_);
  set_parameter_description("Topic for Starlink packets");
  define_parameter("DividePacket_name", &mod_class::dividePacketName_);
  set_parameter_description("Name of DividePacket module");
  define_parameter("qos", &mod_class::qos_);
  set_parameter_description("QoS level for MQTT");
  define_parameter("is_starlink", &mod_class::isStarlink_);
  set_parameter_description("Set true if this module handles not critical packets only via Starlink");
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus PassTelemetry::mod_initialize() {
  if (exist_module(mosquittoManagerName_)) {
    get_module_NC(mosquittoManagerName_, &mosquittoManager_);
  }
  else {
    std::cerr << "MosquittoManager module is not found." << std::endl;
    return AS_ERROR;
  }
  auto mosq = mosquittoManager_->getMosquittoIO();
  if (!mosq) {
    std::cerr << "MosquittoIO in the MosquittoManager is nullptr." << std::endl;
    return AS_ERROR;
  }
  if (exist_module(dividePacketName_)) {
    get_module_NC(dividePacketName_, &dividePacket_);
    subsystem_ = dividePacket_->GetSubsystem();
  }
  else {
    std::cerr << "ReceiveStatusFromDAQComputer module is not found." << std::endl;
    return AS_ERROR;
  }
  return AS_OK;
}
ANLStatus PassTelemetry::mod_analyze() {
  if (!mosquittoManager_) {
    std::cerr << "PassTelemetry::mod_analyze: MosquittoManager is nullptr." << std::endl;
    return AS_OK;
  }
  if (!dividePacket_) {
    std::cerr << "PassTelemetry::mod_analyze: ReceiveStatusFromDAQComputer is nullptr." << std::endl;
    return AS_OK;
  }
  auto mosq = mosquittoManager_->getMosquittoIO();
  if (!mosq) {
    std::cerr << "PassTelemetry::mod_analyze: MosquittoIO in the MosquittoManager is nullptr." << std::endl;
    return AS_OK;
  }
  if (dividePacket_->IsEmpty(isStarlink_)) {
    return AS_OK;
  }
  auto packet = dividePacket_->GetLastPacket(isStarlink_); // TODO: Need to check the size of the packet not to block the main chain.
  if (!packet) {
    std::cerr << "PassTelemetry::mod_analyze: Packet is nullptr. Skipping this packet." << std::endl;
    dividePacket_->PopPacket(isStarlink_);
    return AS_OK;
  }

  outStr_.clear();
  packet->construct(outStr_);
  const auto link_type = mosquittoManager_->getLinkType();
  if (link_type == CommunicationLinkType::STARLINK) {
    MosquittoIO<std::string>::HandleError(mosq->Publish(outStr_, starlinkTopic_, qos_));
    if (chatter_ > 0) {
      std::cout << "PassTelemetry::mod_analyze: Published packet to " << starlinkTopic_ << std::endl;
    }
  }
  else if (link_type == CommunicationLinkType::IRIDIUM && !isStarlink_) {
    MosquittoIO<std::string>::HandleError(mosq->Publish(outStr_, topic_, qos_));
    if (chatter_ > 0) {
      std::cout << "PassTelemetry::mod_analyze: Published packet to " << topic_ << std::endl;
    }
  }

  dividePacket_->PopPacket(isStarlink_);
  return AS_OK;
}
} // namespace gramsballoon::pgrams