#include "SPIManager.hh"

using namespace anlnext;
using namespace gramsballoon;

namespace gramsballoon::pgrams {

SPIManager::SPIManager() {
  interface_ = std::make_shared<SPIInterface>();
}

SPIManager::~SPIManager() {
  Cleanup_libMPSSE();
  interface_.reset();
};

ANLStatus SPIManager::mod_define() {
  define_parameter("channel", &mod_class::channel_);
  define_parameter("baudrate", &mod_class::baudrate_);
  define_parameter("spi_config_options", &mod_class::spiConfigOptions_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}

ANLStatus SPIManager::mod_initialize() {
  const std::string send_telem_md = "SendTelemetry";
  if (exist_module(send_telem_md)) {
    get_module_NC(send_telem_md, &sendTelemetry_);
  }
  Init_libMPSSE();
  interface_->setBaudrate(baudrate_);
  interface_->setConfigOptions(spiConfigOptions_);
  FT_STATUS status = interface_->Open(channel_);
  if (status != FT_OK) {
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
  FT_STATUS status = interface_->Close();
  if (status != FT_OK) {
    std::cerr << "SPI_CloseChannel failed: status = " << status << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR); // TODO: To be implemented
    }
  }
  return AS_OK;
}

} // namespace gramsballoon::pgrams
