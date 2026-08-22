#include "ControlToFBias.hh"
#include "SendTelemetry.hh"
#include "ToFBiasController.hh"
#include <thread>
using namespace anlnext;
namespace gramsballoon::pgrams {
ControlToFBias::ControlToFBias() : sendTelemetry_(nullptr), controller_(nullptr) {}

anlnext::ANLStatus ControlToFBias::mod_define() {
  define_parameter("serial_path", &mod_class::path_);
  define_parameter("chatter", &mod_class::chatter_);
  define_parameter("minimum_duration_sec", &mod_class::minDurationSec_);
  return AS_OK;
}

ANLStatus ControlToFBias::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }

  duration_ = std::chrono::seconds(minDurationSec_);

  controller_ = std::make_shared<ToFBiasController>(path_);
  const int init_res = controller_->initialize();
  if (init_res != 0) {
    std::cerr << "Error in opening file " << path_ << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
    }
    return AS_OK;
  }
  index_ = 0;
  return AS_OK;
}

ANLStatus ControlToFBias::mod_begin_run() {
  if (controller_) {
    const int ret = controller_->enableDataStream();
    if (!ret) {
      std::cerr << module_id() << ": error in enable data stream" << std::endl;
      if (sendTelemetry_) { sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR); }
      return AS_OK;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1)); // wait for new data.
  }
  return AS_OK;
}
ANLStatus ControlToFBias::mod_end_run() {
  if (controller_) {
    const int ret = controller_->disableDataStream();
    if (ret < 0) {
      if (sendTelemetry_) sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
      return AS_OK;
    }
  }
  return AS_OK;
}
ANLStatus ControlToFBias::mod_analyze() {
  if (!controller_) {
    return AS_OK;
  }
  const auto now = std::chrono::steady_clock::now();
  if (index_ != lastIndex_) {
    lastReceivedTime_ = now;
  }
  else if (now - lastReceivedTime_ > duration_) {
    std::cerr << module_id() << ": Exceeds last duration" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
    }
    std::cout << "Error in " << module_id() << ", Reconnecting..." << std::endl;
    controller_->initialize();
    return AS_OK;
  }
  adcData_ = controller_->AdcData();
  dacData_ = controller_->DacData();
  tempData_ = controller_->TempData();
  if (chatter_ > 0) {
    std::cout << "adcData_: ";
    for (size_t i = 0; i < adcData_.size(); i++) {
      std::cout << adcData_[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "dacData_: ";
    for (size_t i = 0; i < dacData_.size(); i++) {
      std::cout << dacData_[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "dacData_: ";
    for (size_t i = 0; i < tempData_.size(); i++) {
      std::cout << tempData_[i] << " ";
    }
    std::cout << std::endl;
  }
  return AS_OK;
}
ANLStatus ControlToFBias::mod_finalize() {
  controller_.reset();
  return AS_OK;
}

int ControlToFBias::setTrimVoltage(uint32_t channel0, uint32_t channel1, uint32_t channel2, uint32_t channel3, uint32_t voltage) {
  return singleton_self()->controller_->setTrimVoltage(channel0, channel1, channel2, channel3, voltage);
}

int ControlToFBias::setDefaultTemperature(uint32_t channel, int temperature) {
  return singleton_self()->controller_->setDefaultTemperature(channel, temperature);
}

int ControlToFBias::enableTemperatureCon() {
  return singleton_self()->controller_->enableTemperatureControl();
}

int ControlToFBias::disableTemperatureCon() {
  return singleton_self()->controller_->disableTemperatureControl();
}

int ControlToFBias::enableDCDC(uint8_t channel) {
  return singleton_self()->controller_->enableDCDC(channel);
}

int ControlToFBias::disableDCDC(uint8_t channel) {
  return singleton_self()->controller_->disableDCDC(channel);
}
} // namespace gramsballoon::pgrams