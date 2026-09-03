#include "EncodedSerialCommunicator.hh"
#include <chrono>
#include <thread>
using namespace anlnext;
namespace gramsballoon::pgrams {

ANLStatus EncodedSerialCommunicator::mod_define() {
  define_parameter("filename", &mod_class::filename_);
  define_parameter("sleep_for_usec", &mod_class::sleepForusec_);
  define_parameter("timeout_usec", &mod_class::timeoutUsec_);
  define_parameter("baudrate", &mod_class::baudrate_);
  define_parameter("mode", &mod_class::mode_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus EncodedSerialCommunicator::mod_initialize() {
  if (timeoutUsec_ < 0) {
    std::cerr << "Error in EncodedSerialCommunicator::mod_initialize: timeoutUsec_ < 0" << std::endl;
    return AS_ERROR;
  }
  esc_ = std::make_shared<EncodedSerialCommunication>(filename_, baudrate_, mode_);
  esc_->setTimeout(std::chrono::microseconds(timeoutUsec_));
  esc_->initialize();
  return AS_OK;
}
ANLStatus EncodedSerialCommunicator::mod_finalize() {
  esc_.reset();
  return AS_OK;
}
int EncodedSerialCommunicator::SendComAndGetData(std::string_view command, std::string &data, int max_length) {
  esc_->Write(command);
  std::this_thread::sleep_for(std::chrono::microseconds(sleepForusec_));
  const int ret = esc_->ReadDataUntilSpecificStr(data, "\r", max_length);
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