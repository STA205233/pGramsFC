#include "GetEnvironmentalData.hh"
#include "I2CInterface.hh"
#include "bme68x.h"
#include <chrono>
#include <thread>

using namespace anlnext;

namespace gramsballoon::pgrams {

GetEnvironmentalData::GetEnvironmentalData() {
  bme680io_ = std::make_shared<BME680IO>();
  interface_ = std::make_shared<I2CInterface>();
}

GetEnvironmentalData::~GetEnvironmentalData() = default;

ANLStatus GetEnvironmentalData::mod_define() {
  define_parameter("I2C_manager_name", &mod_class::I2CManagerName_);
  define_parameter("chatter", &mod_class::chatter_);

  return AS_OK;
}

ANLStatus GetEnvironmentalData::mod_pre_initialize() {
  const std::string send_telem_md = "SendTelemetry";
  if (exist_module(send_telem_md)) {
    get_module_NC(send_telem_md, &sendTelemetry_);
  }

  if (exist_module(I2CManagerName_)) {
    get_module_NC(I2CManagerName_, &I2CManager_);
  }
  else {
    std::cerr << "I2C manager does not exist. Module name = " << I2CManagerName_ << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
  }

  return AS_OK;
}

ANLStatus GetEnvironmentalData::mod_initialize() {
  if (!I2CManager_) {
    std::cerr << "I2C manager is not initialized." << std::endl;
    return AS_ERROR;
  }
  auto interface_ = I2CManager_->Interface();
  if (!interface_) {
    std::cerr << "I2C interface is not initialized." << std::endl;
    //if (sendTelemetry_) {
    //  sendTelemetry_->getErrorManager()->setError(ErrorType::I2C_INTERFACE_ERROR); // TODO: define a new error type for I2C interface error
    //}
    return AS_ERROR;
  }
  bme680io_->setup(interface_.get());
  return AS_OK;
}

ANLStatus GetEnvironmentalData::mod_analyze() {
  const int status = bme680io_->getData();
  if (status != 0) {
    std::cerr << "Failed to get environmental data." << std::endl;
    setDataAquisitionError();
  }
  if (chatter_ >= 1) {
    bme680io_->printData();
  }
  pressure_ = bme680io_->SensorData()->pressure;
  humidity_ = bme680io_->SensorData()->humidity;
  temperature_ = bme680io_->SensorData()->temperature;
  return AS_OK;
}

ANLStatus GetEnvironmentalData::mod_finalize() {
  return AS_OK;
}

void GetEnvironmentalData::setDataAquisitionError() {
  if (sendTelemetry_ == nullptr) return;
  // TODO: consider error to fail data acquisition
}

} // namespace gramsballoon::pgrams
