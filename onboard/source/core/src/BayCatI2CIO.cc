#include "BayCatI2CIO.hh"
#include <iostream>
namespace gramsballoon::pgrams {
int BayCatI2CIO::applySetting() {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  return 0;
}
int BayCatI2CIO::Open(int channel) {
  if (IsOpen()) {
    return 0;
  }
  const auto status = apiStatus();
  if (status != 1) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (I2CIsAvailable(I2C_BUS_TYPE_PRIMARY) != API_OK) {
    std::cerr << "I2C bus is not available" << std::endl;
    return -1;
  }
  else {
    setIsOpen(true);
  }
  return 0;
}
int BayCatI2CIO::Close() {
  if (!IsOpen()) {
    return 0;
  }
  setIsOpen(false);
  return 0;
}

int BayCatI2CIO::WriteThenRead(uint16_t address, const uint8_t *writeBuffer, uint32_t wsize, uint8_t *readBuffer, uint32_t rsize) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (wsize <= 0 || rsize <= 0) {
    std::cerr << "Invalid size: wsize = " << wsize << ", rsize = " << rsize << std::endl;
    return -1;
  }
  const int ret = Write(address, writeBuffer, wsize);
  if (ret != API_OK) {
    std::cerr << "I2CWriteThenRead failed: " << ret << std::endl;
    return ret;
  }
  const int ret_read = Read(address, readBuffer, rsize);
  if (ret_read != API_OK) {
    std::cerr << "I2CWriteThenRead failed: " << ret_read << std::endl;
    return ret_read;
  }
  return 0;
}

int BayCatI2CIO::Write(uint16_t address, const uint8_t *writeBuffer, uint32_t size) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (size <= 0) {
    std::cerr << "Invalid size: size = " << size << std::endl;
    return -1;
  }
  writeBuffer_.resize(size);
  writeBuffer_.assign(writeBuffer, writeBuffer + size);
  const int ret = I2CWriteAddress(I2C_BUS_TYPE_PRIMARY, static_cast<unsigned char>(address), writeBuffer_.data(), size);
  return 0;
}
int BayCatI2CIO::Read(uint16_t address, uint8_t *readBuffer, uint32_t size) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (size <= 0) {
    std::cerr << "Invalid size: size = " << size << std::endl;
    return -1;
  }
  const int ret = I2CReadAddress(I2C_BUS_TYPE_PRIMARY, static_cast<unsigned char>(address), readBuffer, size);
  return ret;
}
} // namespace gramsballoon::pgrams