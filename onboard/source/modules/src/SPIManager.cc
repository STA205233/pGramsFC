#include "SPIManager.hh"
#include "PDUCSMapping.hh"
#include "SPIInterfaceMultiplexer.hh"
#include <cstdint>
#include <memory>
#ifdef USE_FT232H
#include "FT232HIO.hh"
#endif

#ifdef USE_BAYCAT
#include "BayCatSPIIO.hh"
#endif
using namespace anlnext;

namespace gramsballoon::pgrams {

SPIManager::SPIManager() = default;

SPIManager::~SPIManager() {
  interface_.reset();
};

ANLStatus SPIManager::mod_define() {
  define_parameter("channel", &mod_class::channel_);
  define_parameter("baudrate", &mod_class::baudrate_);
  define_parameter("spi_config_options", &mod_class::spiConfigOptions_);
  define_parameter("spi_control_type", &mod_class::spiControlType_);
  define_parameter("use_multiplexer", &mod_class::useMultiplexer_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus SPIManager::mod_pre_initialize() {
  std::shared_ptr<SPIInterface> base_interface = nullptr;
  if (spiControlType_ == "baycat") {
#ifdef USE_BAYCAT
    base_interface = std::make_shared<BayCatSPIIO>();
#else
    std::cerr << "BayCat SPI control type is not supported in this build." << std::endl;
    return AS_ERROR;
#endif
  }
  else if (spiControlType_ == "ft232h") {
#ifdef USE_FT232H
    base_interface = std::make_shared<FT232HIO>();
#else
    std::cerr << "FT232H SPI control type is not supported in this build." << std::endl;
    return AS_ERROR;
#endif
  }
  else {
    std::cerr << "Invalid SPI control type: " << spiControlType_ << std::endl;
    return AS_ERROR;
  }

  base_interface->setBaudrate(baudrate_);
  base_interface->setConfigOptions(spiConfigOptions_);
  const int status = base_interface->Open(channel_);
  if (status != 0) {
    std::cerr << "SPI_OpenChannel failed: status = " << status << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR); // TODO: To be implemented
    }
  }

  if (useMultiplexer_) {
    auto mul_interface = std::make_shared<SPIInterfaceMultiplexer>();
    mul_interface->setBaseInterface(base_interface);
    mul_interface->setMappingChipSelect(std::make_unique<PDUCSMapping>(0x1f0000));
    interface_ = mul_interface;
  }
  else {
    interface_ = base_interface;
  }

  if (!interface_) {
    std::cerr << "Failed to create SPI interface" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR); // TODO: To be implemented
    }
    return AS_ERROR;
  }

  return AS_OK;
}
ANLStatus SPIManager::mod_initialize() {
  const std::string send_telem_md = "SendTelemetry";
  if (exist_module(send_telem_md)) {
    get_module_NC(send_telem_md, &sendTelemetry_);
  }
  return AS_OK;
}

ANLStatus SPIManager::mod_analyze() {
  return AS_OK;
}

ANLStatus SPIManager::mod_finalize() {
  if (!interface_) {
    return AS_OK;
  }
  int status = interface_->Close();
  if (status != 0) {
    std::cerr << "SPI_CloseChannel failed: status = " << status << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR); // TODO: To be implemented
    }
  }
  return AS_OK;
}

int SPIManager::controlGPIO(uint32_t cs, bool value) {
  if (!interface_) {
    return -1;
  }
  const uint32_t val = (static_cast<uint32_t>(value) << cs);
  return interface_->controlGPIOBit(cs, val);
}

} // namespace gramsballoon::pgrams
