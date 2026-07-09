#include "ADC128S102IO.hh"
namespace gramsballoon::pgrams {
float ADC128S102IO::convertVoltage(uint16_t value) const {
  return LSB_ * value + halfLSB_;
}
float ADC128S102IO::getCurrentVoltage(int ch) {
  const uint8_t reg_value = (ch << 3) & (0xFF);
  writeBuffer_[0] = reg_value;
  writeBuffer_[1] = 0;
  const int status = spiInterface_->WriteAndRead(cs_, writeBuffer_, 2, readBuffer_);
  if (status < 0) {
    std::cerr << "Error in getCurrentVoltage: " << status << std::endl;
    errorCode_ = status;
    return 0.0f;
  }
  const uint16_t val = ((readBuffer_[0] & 0x00ff) << 8) | (readBuffer_[1] & 0xffff);
  return convertVoltage(val);
}
bool ADC128S102IO::getAllADCs(std::array<float, ADC128S102IO::MaxChannelsPerADC()> &dest) {
  bool has_error = false;
  for (int i = 0; i < MaxChannelsPerADC(); ++i) {
    const auto vol = getCurrentVoltage(i);
    if (isError()) {
      return false;
    }
    dest[i] = vol;
  }
  return true;
}
} // namespace gramsballoon::pgrams