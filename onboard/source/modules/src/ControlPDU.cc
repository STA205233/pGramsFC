#include "ControlPDU.hh"
#include "PDUCommandCSMapping.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ControlPDU::ControlPDU() : BasicModule(), spiManager_(nullptr), sendTelemetry_(nullptr), dac_(nullptr) {}
ANLStatus ControlPDU::mod_define() {
  define_parameter("SPIManager_name", &mod_class::spiManagerName_);
  set_parameter_description("SPIManager name for accessing SPI interface");
  return AS_OK;
}

ANLStatus ControlPDU::mod_initialize() {
  if (exist_module(spiManagerName_)) {
    get_module_NC(spiManagerName_, &spiManager_);
  }
  else {
    std::cerr << "ControlPDU: SPIManager module not found: " << spiManagerName_ << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
    return AS_ERROR;
  }
  dac_ = std::make_shared<DAC121S101IO>(); // TODO: chip select pin number
  auto csMapping_ = std::make_unique<PDUCommandCSMapping>();
  maxPDUChannels_ = csMapping_->MaximumCh();
  auto interface = spiManager_->Interface();
  if (interface) {
    dac_->setSPIInterface(interface);
    interface->setMappingChipSelect(std::move(csMapping_));
  }
  else {
    std::cerr << "ControlPDU: SPI interface is not initialized" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR);
    }
    return AS_ERROR;
  }
  return AS_OK;
}

int ControlPDU::initializeDAC() {
  if (singleton_self()->dac_->isSPIInterfaceSet()) {
    singleton_self()->dac_->setOperationMode(DAC121S101Mode::DAC121S101_MODE_NORMAL);
    singleton_self()->dac_->setVoltage(0.0);
    int status = 0;

    for (int cs = 0; cs < maxPDUChannels_; ++cs) {
      singleton_self()->dac_->setCS(cs);
      status = singleton_self()->dac_->applySetting();
      if (status < 0) {
        std::cerr << "Failed to apply DAC setting: " << status << std::endl;
        if (singleton_self()->sendTelemetry_) {
          singleton_self()->sendTelemetry_->getErrorManager()->setError(ErrorType::DAC_SETTING_ERROR);
        }
      }
    }
    return status;
  }
  return -1;
}
} // namespace gramsballoon::pgrams