#include "MCP2210IO.hh"
extern "C" {
#include "mcp2210_api.h"
}
#include <algorithm>
#include <cstdint>
#include <iostream>
namespace gramsballoon::pgrams {
MCP2210IO::MCP2210IO() : handler_(-1), spiMode_(0) {}
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

int MCP2210IO::Close() {
  const auto ret = close_device(handler_);
  if (ret >= 0) {
    setIsOpen(false);
    handler_ = -1;
  }
  return ret;
}

void MCP2210IO::setConfigOptions(unsigned int configOptions) {
  SPIInterface::setConfigOptions(configOptions);
  spiMode_ = configOptions;
}

int MCP2210IO::WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer, bool csControl) {
  if (!IsOpen()) {
    return -1;
  }
  if (csControl) {
    const int ret_gpio = controlGPIO(cs, false);
    if (ret_gpio < 0) {
      controlGPIO(cs, true);
      return -1;
    }
  }
  auto ret = spi_data_xfer(handler_, writeBuffer, readBuffer, size, spiMode_, Baudrate(), ALL_HIGH, ALL_HIGH, 0, 0, 0, 0);

  if (ret < 0) {
    std::cerr << "Error: SPI write failed " << ret << std::endl;
  }
  else if (ret != static_cast<int>(size)) {
    std::cerr << "MCP2210IO:: size of transmission is incorrect" << std::endl;
    ret = -1; // if inconstency size of transmission
  }
  if (csControl) {
    const auto ret_gpio = controlGPIO(cs, true);
    if (ret_gpio < 0) {
      return ret_gpio;
    }
  }
  return ret;
}

int MCP2210IO::WriteThenRead(int cs, const uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer, unsigned int rsize, bool csControl) {
  const auto sz = static_cast<size_t>(rsize) + static_cast<size_t>(wsize);
  if (sz > NUM_BUF) {
    std::cerr << "MCP2210IO::WriteThenRead: Requested size is larger than expected " << std::endl;
    return -1;
  }
  wbuffer_.fill(0);
  rbuffer_.fill(0);
  std::copy(writeBuffer, writeBuffer + wsize, wbuffer_.begin());
  const int ret = WriteAndRead(cs, wbuffer_.data(), sz, rbuffer_.data(), csControl);
  std::copy(rbuffer_.begin() + wsize, rbuffer_.begin() + sz, readBuffer);
  return ret;
}

int MCP2210IO::Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) {
  if (size > NUM_BUF) {
    std::cerr << "MCP2210IO::Write: Requested size is larger than expected " << std::endl;
    return -1;
  }
  std::copy(writeBuffer, writeBuffer + size, wbuffer_.begin());
  return WriteAndRead(cs, wbuffer_.data(), size, rbuffer_.data(), csControl);
}

int MCP2210IO::MaximumCh() const { return 8; }

int MCP2210IO::controlGPIO(int cs, bool val) {
  if (cs < 0) {
    std::cout << "MCP2210IO::controlGPIO: negative cs value" << std::endl;
    return -1;
  }
  const int cs_val = (1 << cs);
  const int val_bit = val ? cs_val : 0;
  const auto ret = gpio_write(handler_, val_bit, cs_val);
  if (ret < 0) {
    std::cerr << "MCP2210IO::controlGPIO: Control GPIO failed" << ret << std::endl;
  }
  return ret;
}
int MCP2210IO::controlGPIOBit(uint32_t cs, uint32_t state) {
  return gpio_write(handler_, state, cs);
}
} // namespace gramsballoon::pgrams