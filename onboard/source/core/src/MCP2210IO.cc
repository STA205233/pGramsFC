#include "MCP2210IO.hh"
#include "mcp2210_api.h"
namespace gramsballoon::pgrams {
MCP2210IO::MCP2210IO() = default;
MCP2210IO::~MCP2210IO() = default;
int MCP2210IO::Open(int, const char *path) {
  const auto ret = open_device(path);
  if (ret < 0) {
    setIsOpen(false);
    return ret;
  }
  setIsOpen(true);
  handler_ = ret;
  return 0;
}
void MCP2210IO::setConfigOptions(unsigned int configOptions) {
  SPIInterface::setConfigOptions(configOptions);
  spiMode_ = configOptions;
}
int MCP2210IO::WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer, bool csControl) {
  if (!IsOpen()) {
    return -1;
  }
  const auto ret = spi_data_xfer(handler_, writeBuffer, readBuffer, size, spiMode_, Baudrate(), );
}
} // namespace gramsballoon::pgrams