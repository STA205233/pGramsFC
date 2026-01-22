#include "GetPDUADC.hh"
using namespace anlnext;

namespace gramsballoon::pgrams {
ANLStatus GetPDUADC::mod_define() {
  set_module_description("Module to get PDU ADC readings via SPI");
  define_parameter("SPIManager_name", &mod_class::spiManagerName_);
  set_parameter_description("Name of the SPIManager module");
  define_parameter("channel", &mod_class::channel_);
  set_parameter_description("ADC channel (Chip Select)");
  define_parameter("supply_voltage", &mod_class::supplyVoltage_, 1.0, "V");
  set_parameter_description("Supply voltage for ADC");
  set_parameter_default_string("3.3");
  set_parameter_unit(1.0, "V");
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus GetPDUADC::mod_initialize() {
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
  adcInterface_ = std::make_shared<ADC128S102IO>(supplyVoltage_);
  adcInterface_->setCS(channel_);
  adcInterface_->setSPIInterface(spiManager_->Interface());
  return AS_OK;
}
ANLStatus GetPDUADC::mod_analyze() {
  bool failed = false;
  for (int ch = 0; ch < 8; ++ch) {
    const float voltage = adcInterface_->getCurrentVoltage(ch);
    if (voltage < 0) {
      std::cerr << "Error reading voltage from channel " << ch << std::endl;
      failed = true;
      continue;
    }
    if (chatter_ > 0) {
      std::cout << "Channel " << ch << ": " << voltage << " V" << std::endl;
    }
  }
  if (failed) {
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::ADC_READING_ERROR);
    }
    return AS_ERROR;
  }
  return AS_OK;
}
ANLStatus GetPDUADC::mod_finalize() {
  adcInterface_.reset();
  return AS_OK;
}
} // namespace gramsballoon::pgrams