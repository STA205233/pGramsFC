#include "GetLabJackData.hh"
#include <iostream>
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus GetLabJackData::mod_define() {
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus GetLabJackData::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  labjackIO_ = std::make_shared<LabJackIO>();
  const auto err = labjackIO_->open();
  if (err != 0) {
    std::cerr << "Failed to open LabJack device: " << err << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::LABJACK_CONNECT_ERROR);
    }
    return AS_ERROR;
  }
  return AS_OK;
}
ANLStatus GetLabJackData::mod_analyze() {
  if (!labjackIO_) {
    return AS_ERROR;
  }
  const auto err1 = labjackIO_->read(LabJackAddresses::AIN<0>, analogIn_[0]);
  const auto err2 = labjackIO_->read(LabJackAddresses::AIN<1>, analogIn_[1]);
  if (err1 != 0 || err2 != 0) {
    if (chatter_ > 1) {
      std::cerr << "Error reading from LabJack device: " << err1 << ", " << err2 << std::endl;
    }
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::LABJACK_READ_ERROR);
    }
    analogIn_.fill(0.0);
    return AS_ERROR;
  }
  if (chatter_ > 4) {
    std::cout << "AIN0: " << analogIn_[0] << ", AIN1: " << analogIn_[1] << std::endl;
  }
  return AS_OK;
}
ANLStatus GetLabJackData::mod_finalize() {
  if (labjackIO_) {
    const auto err = labjackIO_->close();
    if (err != 0) {
      std::cerr << "Failed to close LabJack device: " << err << std::endl;
      return AS_ERROR;
    }
  }
  return AS_OK;
}
} // namespace gramsballoon::pgrams