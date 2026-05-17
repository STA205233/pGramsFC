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
  if (err1 != 0) {
    char err_str[LJM_STRING_MAX_SIZE];
    LJM_ErrorToString(err1, err_str);
    std::cerr << "Error reading from LabJack device: " << err_str << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::LABJACK_READ_ERROR);
    }
    analogIn_[0] = 0.0;
  }
  const auto err2 = labjackIO_->read(LabJackAddresses::AIN<1>, analogIn_[1]);
  if (err2 != 0) {
    char err_str[LJM_STRING_MAX_SIZE];
    LJM_ErrorToString(err2, err_str);
    std::cerr << "Error reading from LabJack device: " << err_str << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::LABJACK_READ_ERROR);
    }
    analogIn_[1] = 0.0;
  }
  const auto err3 = labjackIO_->read(LabJackAddresses::TEMPERATURE_DEVICE_K, temperatureDevice_);
  if (err3 != 0) {
    char err_str[LJM_STRING_MAX_SIZE];
    LJM_ErrorToString(err3, err_str);
    std::cerr << "Error reading from LabJack device: " << err_str << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::LABJACK_READ_ERROR);
    }
    temperatureDevice_ = 0.0;
  }
  const auto err4 = labjackIO_->read(LabJackAddresses::TEMPERATURE_AIR_K, temperatureAir_);
  if (err4 != 0) {
    char err_str[LJM_STRING_MAX_SIZE];
    LJM_ErrorToString(err4, err_str);
    std::cerr << "Error reading from LabJack device: " << err_str << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::LABJACK_READ_ERROR);
    }
    temperatureAir_ = 0.0;
  }
  if (chatter_ > 4) {
    std::cout << "AIN0: " << analogIn_[0] << ", AIN1: " << analogIn_[1] << ", DEVICE: " << temperatureDevice_ << ", AIR: " << temperatureAir_ << std::endl;
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