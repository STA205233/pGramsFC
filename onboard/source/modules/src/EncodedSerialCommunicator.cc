#include "EncodedSerialCommunicator.hh"
#include <chrono>
#include <thread>
using namespace anlnext;
namespace gramsballoon::pgrams {

ANLStatus EncodedSerialCommunicator::mod_define() {
  define_parameter("filename", &mod_class::filename_);
  define_parameter("timeout_sec", &mod_class::timeout_);
  define_parameter("timeout_usec", &mod_class::timeoutUsec_);
  define_parameter("baudrate", &mod_class::baudrate_);
  define_parameter("mode", &mod_class::mode_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus EncodedSerialCommunicator::mod_initialize() {
  if (timeout_ < 0 || timeoutUsec_ < 0) {
    std::cerr << "Error in EncodedSerialCommunicator::mod_initialize: timeout_ < 0" << std::endl;
    return AS_ERROR;
  }
  esc_ = std::make_shared<EncodedSerialCommunication>(filename_, baudrate_, mode_);
  esc_->initialize();
  return AS_OK;
}
ANLStatus EncodedSerialCommunicator::mod_finalize() {
  esc_.reset();
  return AS_OK;
}
int EncodedSerialCommunicator::SendComAndGetData(std::string_view command, std::string& data, int sleepfor) {
  esc_->Write(command);
  const int ret = esc_->ReadDataUntilSpecificStr(data, "\r");
  if (chatter_ > 0) {
    std::cout << "data: " << data << std::endl;
    std::cout << "ret: " << ret << std::endl;
  }
  return ret;
}
int EncodedSerialCommunicator::SendCommand(std::string_view command) {
  const int ret = esc_->Write(command);
  return ret;
}
} // namespace gramsballoon::pgrams