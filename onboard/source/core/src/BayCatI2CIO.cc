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
int BayCatI2CIO::Open(int) {
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
  I2CSetFrequency(I2C_BUS_TYPE_PRIMARY, I2C_FREQUENCY_100KHZ);
  return 0;
}
int BayCatI2CIO::Close() {
  if (!IsOpen()) {
    return 0;
  }
  setIsOpen(false);
  return 0;
}

int BayCatI2CIO::ReadRegister(uint16_t address, uint8_t reg_address, uint8_t *readBuffer, uint32_t rsize) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (rsize <= 0) {
    std::cerr << "Invalid size: rsize = " << rsize << std::endl;
    return -1;
  }
  if (I2CIsAvailable(I2C_BUS_TYPE_PRIMARY) != API_OK) {
    std::cerr << "I2C bus is not available" << std::endl;
    return -1;
  }
  buffer_.clear();
  buffer_.resize(rsize + 1);
  buffer_[0] = static_cast<unsigned char>(reg_address);
  const int ret_read = I2CReadAddress(I2C_BUS_TYPE_PRIMARY, address, buffer_.data(), rsize);
  if (ret_read != API_OK) {
    std::cerr << "I2CReadRegister failed: " << ret_read << std::endl;
    return ret_read;
  }
  if (rsize > 0) {
    std::copy(buffer_.begin(), buffer_.end() - 1, readBuffer);
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
  buffer_.clear();
  buffer_.assign(writeBuffer, writeBuffer + size);
  const int ret = I2CWriteAddress(I2C_BUS_TYPE_PRIMARY, static_cast<unsigned char>(address), buffer_.data(), size);
  if (ret != API_OK) {
    std::cerr << "I2CWriteAddress failed: " << ret << std::endl;
    return -ret;
  }
  return ret;
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
  if (ret != API_OK) {
    std::cerr << "I2CReadAddress failed: " << ret << std::endl;
    return -ret;
  }
  return ret;
}
} // namespace gramsballoon::pgrams