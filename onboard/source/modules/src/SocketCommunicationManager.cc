#include "SocketCommunicationManager.hh"
#include <sys/select.h>
#include <chrono>
#include <thread>
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus SocketCommunicationManager::mod_define() {
  set_module_description("This module is responsible for socket communication, mainly between the DAQ computer");
  define_parameter("ip", &mod_class::ip_);
  set_parameter_description("ip address of the server");
  define_parameter("port", &mod_class::port_);
  set_parameter_description("port of the server");
  define_parameter("handle_SIGPIPE", &mod_class::handleSigpipe_);
  set_parameter_description("Flag for handling SIGPIPE. This process is normally necessary once per process.");
  define_parameter("timeout", &mod_class::timeout_);
  set_parameter_description("Receive Timeout");
  set_parameter_unit(1.0, "msec");
  define_parameter("subsystem", &mod_class::subsystemInt_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus SocketCommunicationManager::mod_pre_initialize() {
  if (handleSigpipe_)
    SocketCommunication::HandleSIGPIPE();
  return AS_OK;
}
ANLStatus SocketCommunicationManager::mod_initialize() {
  subsystem_ = magic_enum::enum_cast<Subsystem>(subsystemInt_).value_or(Subsystem::UNKNOWN);
  if (subsystem_ == Subsystem::UNKNOWN) {
    std::cerr << "Error in " << module_id() << "::mod_initialize: Invalid subsystem value " << subsystemInt_ << ". Setting to UNKNOWN." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::OTHER_ERRORS);
    }
  }
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize SendTelemetry module is not found." << std::endl;
  }
  if (exist_module("IoContextManager")) {
    get_module_NC("IoContextManager", &ioContextManager_);
    ioContext_ = ioContextManager_->getIoContext();
    if (!ioContext_) {
      std::cerr << module_id() << "::mod_initialize IoContextManager is not initialized." << std::endl;
      return AS_ERROR;
    }
  }
  else {
    std::cerr << module_id() << "::mod_initialize IoContextManager module is not found." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
    return AS_ERROR;
  }
  try {
    socketCommunication_ = std::make_shared<SocketCommunication>(ioContext_, port_);
  }
  catch (...) {
    socketCommunication_ = nullptr;
  }
  if (!socketCommunication_) {
    std::cerr << module_id() << "::mod_initialize SocketCommunication is not initialized." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqComErrorType(subsystem_, true));
    }
    return AS_ERROR;
  }
  if (timeout_ > 0) {
    std::cout << module_id() << "::mod_initialize Timeout is set to " << timeout_ << " msec." << std::endl;
    socketCommunication_->setTimeout(timeout_);
  }
  socketCommunication_->accept();
  return AS_OK;
}
ANLStatus SocketCommunicationManager::mod_analyze() {
  return AS_OK;
}
ANLStatus SocketCommunicationManager::mod_finalize() {
  if (socketCommunication_) {
    socketCommunication_->close();
  }
  return AS_OK;
}
int SocketCommunicationManager::sendAndWaitForAck(const uint8_t *buf, size_t n, const uint8_t *ack, size_t ack_n) {
  if (!singleton_self()->socketCommunication_) {
    return -1;
  }
  const int send_result = singleton_self()->socketCommunication_->send(buf, n);
  if (send_result < 0) {
    return send_result;
  }
  if (chatter_ > 2) {
    std::cout << module_id() << "::sendAndWaitForAck: Sent data: ";
    for (size_t i = 0; i < n; ++i) {
      std::cout << static_cast<int>(buf[i]) << " ";
    }
    std::cout << std::endl;
  }
  singleton_self()->ackBuffer_.clear();
  size_t acksz;
  for (int i = 0; i < 10; i++) {
    singleton_self()->receive(singleton_self()->ackBuffer_);
    acksz = singleton_self()->ackBuffer_.size();
    if (acksz == 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      continue;
    }
    if (acksz != ack_n) {
      std::cerr << module_id() << "::sendAndWaitForAck: Acknowledgement size mismatch. Expected: " << ack_n << ", Received: " << acksz << std::endl;
      return -ack_n;
    }
    break;
  }
  bool failed = false;
  if (acksz == 0) {
    std::cerr << module_id() << "::sendAndWaitForAck: No data is received." << std::endl;
    failed = true;
  }
  for (size_t i = 0; i < acksz; ++i) {
    if (singleton_self()->ackBuffer_[i] != ack[i]) {
      std::cerr << module_id() << "::sendAndWaitForAck: Acknowledgement data mismatch at " << i << ". Expected: " << static_cast<int>(ack[i]) << ", Received: " << static_cast<int>(singleton_self()->ackBuffer_[i]) << std::endl;
      failed = true;
    }
  }
  if (failed) {
    return -1;
  }
  return send_result;
}
int SocketCommunicationManager::receive(std::vector<uint8_t> &data) {
  if (!singleton_self()->socketCommunication_) {
    return -1;
  }
  const int ret = singleton_self()->socketCommunication_->receive(data);
  if (ret <= 0) {
    return ret;
  }
  data.resize(ret);
  return ret;
}
} // namespace gramsballoon::pgrams