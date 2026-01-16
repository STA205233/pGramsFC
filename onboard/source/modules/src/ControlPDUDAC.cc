#include "ControlPDUDAC.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus ControlPDUDAC::mod_define() {
  define_parameter("SPIManager_name", &mod_class::spiManagerName_);
  define_parameter("channel", &mod_class::channel_);
  define_parameter("supply_voltage", &mod_class::supplyVoltage_, 1.0, "V");
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus ControlPDUDAC::mod_initialize() {
  if (exist_module(spiManagerName_)) {
    get_module_NC(spiManagerName_, &spiManager_);
  }
  else {
    std::cerr << spiManagerName_ << " does not exist." << std::endl;
    spiManager_ = nullptr;
    return AS_ERROR;
  }
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  else {
    std::cerr << "SendTelemetry module does not exist." << std::endl;
  }
  dacInterface_ = std::make_shared<DAC121S101IO>(supplyVoltage_);
  dacInterface_->setSPIInterface(spiManager_->Interface());
  return AS_OK;
}
ANLStatus ControlPDUDAC::mod_analyze() {
  if (!dacInterface_) {
    std::cerr << "DAC interface is not initialized." << std::endl;
    return AS_ERROR;
  }
  dacInterface_->setCS(channel_);
  dacInterface_->setVoltage(voltage_);
  const int status = dacInterface_->applySetting();
  if (status != 0) {
    std::cerr << "Error setting voltage on channel " << channel_ << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::DAC_SETTING_ERROR); // TODO: To be implemented
    }
    return AS_ERROR;
  }
  if (chatter_ > 0) {
    std::cout << "Set voltage to " << voltage_ << " V on channel " << channel_ << std::endl;
  }
  return AS_OK;
}
ANLStatus ControlPDUDAC::mod_finalize() {
  dacInterface_.reset();
  return AS_OK;
}
} // namespace gramsballoon::pgrams