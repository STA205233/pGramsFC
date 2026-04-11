#include "SPIManager.hh"
#include "SPIInterfaceMultiplexer.hh"
#ifdef USE_FT232H
#include "FT232HIO.hh"
#endif

#ifdef USE_BAYCAT_SPI
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
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus SPIManager::mod_pre_initialize() {
  std::shared_ptr<SPIInterface> base_interface = nullptr;
  interface_ = std::make_shared<SPIInterfaceMultiplexer>();
  if (spiControlType_ == "baycat") {
#ifdef USE_BAYCAT_SPI
    base_interface = std::make_shared<BayCatSPIIO>();
#else
    std::cerr << "BayCat SPI control type is not supported in this build." << std::endl;
    return AS_ERROR;
#endif
  }
  else if (spiControlType_ == "ft232h") {
    base_interface = std::make_shared<FT232HIO>();
  }
  else {
    std::cerr << "Invalid SPI control type: " << spiControlType_ << std::endl;
    return AS_ERROR;
  }
  interface_->setBaseInterface(std::move(base_interface));
  return AS_OK;
}
ANLStatus SPIManager::mod_initialize() {
  const std::string send_telem_md = "SendTelemetry";
  if (exist_module(send_telem_md)) {
    get_module_NC(send_telem_md, &sendTelemetry_);
  }

  if (!interface_) {
    std::cerr << "Failed to create SPI interface" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR); // TODO: To be implemented
    }
    return AS_ERROR;
  }

  interface_->setBaudrate(baudrate_);
  interface_->setConfigOptions(spiConfigOptions_);
  int status = interface_->Open(channel_);
  if (status != 0) {
    std::cerr << "SPI_OpenChannel failed: status = " << status << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR); // TODO: To be implemented
    }
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

} // namespace gramsballoon::pgrams
