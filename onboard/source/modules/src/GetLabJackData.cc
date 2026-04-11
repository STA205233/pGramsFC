#include "GetLabJackData.hh"
#include <iostream>
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus GetLabJackData::mod_define() {
  define_parameter("range_setting", &mod_class::rangeSetting_);
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
    char err_str[LJM_STRING_MAX_SIZE];
    LJM_ErrorToString(err, err_str);
    std::cerr << "Failed to open LabJack device: " << err_str << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::LABJACK_CONNECT_ERROR);
    }
    return AS_ERROR;
  }
  labjackIO_->printInfo(std::cout);
  labjackIO_->configure(rangeSetting_, 199, 0, 0);
  return AS_OK;
}
ANLStatus GetLabJackData::mod_analyze() {
  if (!labjackIO_) {
    return AS_ERROR;
  }
  if (!labjackIO_->isOpen()) {
    const auto open_result = labjackIO_->open();
    if (open_result != 0) {
      return AS_ERROR;
    }
  }
  const auto err1 = labjackIO_->read(LabJackAddresses::AIN<0>, analogIn_[0]);
  const auto err2 = labjackIO_->read(LabJackAddresses::AIN<1>, analogIn_[1]);
  if (err1 != 0 || err2 != 0) {
    if (chatter_ > 1) {
      char err_str1[LJM_STRING_MAX_SIZE];
      LJM_ErrorToString(err1, err_str1);
      char err_str2[LJM_STRING_MAX_SIZE];
      LJM_ErrorToString(err2, err_str2);
      std::cerr << "Error reading from LabJack device: " << err_str1 << ", " << err_str2 << std::endl;
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
      char err_str[LJM_STRING_MAX_SIZE];
      LJM_ErrorToString(err, err_str);
      std::cerr << "Failed to close LabJack device: " << err_str << std::endl;
      return AS_ERROR;
    }
  }
  return AS_OK;
}
} // namespace gramsballoon::pgrams