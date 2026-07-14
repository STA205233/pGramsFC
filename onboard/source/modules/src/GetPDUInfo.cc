#include "GetPDUInfo.hh"
#include "PDUTelemetryCSMapping.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus GetPDUInfo::mod_define() {
  define_parameter("SPIManager_name", &mod_class::spiManagerName_);
  set_parameter_description("SPIManager name for accessing SPI interface");
  return AS_OK;
}
ANLStatus GetPDUInfo::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  if (exist_module(spiManagerName_)) {
    get_module_NC(spiManagerName_, &spiManager_);
  }
  else {
    std::cerr << "GetPDUInfo: SPIManager module not found: " << spiManagerName_ << std::endl;
    return AS_ERROR;
  }
  auto interface = spiManager_->Interface();
  if (!interface) {
    std::cerr << "GetPDUInfo: SPI interface is not initialized" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR);
    }
    return AS_ERROR;
  }
  adc_ = std::make_shared<ADC128S102IO>(); // TODO: chip select pin number
  auto csMapping = std::make_unique<PDUTelemetryCSMapping>();
  interface->setMappingChipSelect(std::move(csMapping));
  adc_->setSPIInterface(interface);
  return AS_OK;
}
ANLStatus GetPDUInfo::mod_analyze() {
  if (!adc_) {
    std::cerr << "GetPDUInfo: ADC is not initialized" << std::endl;
    return AS_ERROR;
  }
  if (!adc_->isSPIInterfaceSet()) {
    if (!spiManager_) {
      return AS_OK;
    }
    auto interface = spiManager_->Interface();
    if (!interface) {
      std::cerr << "GetPDUInfo: SPI interface is not initialized" << std::endl;
      return AS_ERROR;
    }
    adc_->setSPIInterface(interface);
  }

  for (int cs = 0; cs < numAdcs_; ++cs) {
    adc_->setCS(cs);
    for (int ch = 0; ch < adc_->MaxChannelsPerADC(); ++ch) {
      const float voltage = adc_->getCurrentVoltage(ch);
      voltages_.push_back(voltage);
      std::cout << "PDU Channel " << ch << " (CS " << cs << "): " << voltage << " V" << std::endl;
      if (adc_->isError()) {
        std::cerr << "Error reading voltage for CS " << cs << ", channel " << ch << ": " << adc_->getErrorCode() << std::endl;
        if (sendTelemetry_) {
          sendTelemetry_->getErrorManager()->setError(ErrorType::ADC_ACCESS_ERROR);
        }
      }
    }
  }
  return AS_OK;
}
} // namespace gramsballoon::pgrams