#include "DetectErrorCallbackFromDAQ.hh"
#include "CommunicationCodes.hh"
using namespace anlnext;
using namespace pgrams::communication;
namespace gramsballoon::pgrams {
ANLStatus DetectErrorCallbackFromDAQ::mod_define() {
  define_parameter("dividePacketModuleName", &mod_class::dividePacketModuleName_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus DetectErrorCallbackFromDAQ::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize: SendTelemetry module is not found." << std::endl;
  }
  if (exist_module(dividePacketModuleName_)) {
    get_module(dividePacketModuleName_, &dividePacket_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize: DividePacket module is not found." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
    return AS_ERROR;
  }
  return AS_OK;
}
ANLStatus DetectErrorCallbackFromDAQ::mod_analyze() {
  if (!dividePacket_) {
    return AS_OK;
  }
  if (chatter_ > 1) {
    std::cout << module_id() << "num of pushed packets: " << dividePacket_->GetLastPushedPackets().size() << std::endl;
  }
  for (const auto &packet: dividePacket_->GetLastPushedPackets()) {
    if (!packet) {
      continue;
    }
    if (packet->getContents()->Code() == to_u16(CommunicationCodes::CMN_Command_Error)) {
      std::cout << "Detected DAQ Error Callback packet code: " << static_cast<int>(packet->getContents()->Code()) << "from " << packet->getType() << std::endl;
      if (sendTelemetry_) {
        sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_DAQ_COMMAND_ERROR);
        if (chatter_ > 0) {
          std::cout << module_id() << ": Detected DAQ Error Callback packet." << std::endl;
        }
      }
    }
  }
  return AS_OK;
}
ANLStatus DetectErrorCallbackFromDAQ::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams