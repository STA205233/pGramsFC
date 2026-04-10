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
  if (VSL_I2CIsAvailable(busNumber_) != VL_API_OK) {
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

int BayCatI2CIO::Write(uint16_t address, const uint8_t *writeBuffer, unsigned int size) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (size <= 0) {
    std::cerr << "Invalid size: size = " << size << std::endl;
    return -1;
  }

  return 0;
}

} // namespace gramsballoon::pgrams