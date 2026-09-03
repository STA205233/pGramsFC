#include "ADC128S102IO.hh"
#include <cstdint>
#include <iostream>
namespace gramsballoon::pgrams {
float ADC128S102IO::convertVoltage(uint16_t value) const {
  return LSB_ * value + halfLSB_;
}
uint16_t ADC128S102IO::getCurrentVoltageADC(int ch) {
  const uint8_t reg_value = (ch << 3) & (0xFF);
  writeBuffer_[0] = reg_value;
  writeBuffer_[1] = 0;
  for (int i = 0; i < 2; ++i) { // The address is for next transaction.
    const int status = spiInterface_->WriteAndRead(cs_, writeBuffer_, 2, readBuffer_);
    if (status < 0) {
      std::cerr << "Error in getCurrentVoltage: " << status << std::endl;
      errorCode_ = status;
      return 0.0f;
    }
  }
  const uint16_t val = ((readBuffer_[0] & 0x00ff) << 8) | (readBuffer_[1] & 0xffff);
  return val;
}

float ADC128S102IO::getCurrentVoltage(int ch) {
  return convertVoltage(getCurrentVoltageADC(ch));
}

bool ADC128S102IO::getAllADCs(std::array<float, 8>& dest) {
  for (size_t i = 0; i < MaxChannelsPerADC(); ++i) {
    const auto vol = getCurrentVoltage(i);
    if (isError()) {
      return false;
    }
    dest[i] = vol;
  }
  return true;
}

bool ADC128S102IO::getAllADCs(std::array<uint16_t, 8>& dest) {
  for (size_t i = 0; i < MaxChannelsPerADC(); ++i) {
    const auto vol = getCurrentVoltageADC(i);
    if (isError()) {
      return false;
    }
    dest[i] = vol;
  }
  return true;
}
} // namespace gramsballoon::pgrams