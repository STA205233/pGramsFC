#include "ADC128S102IO.hh"
namespace gramsballoon::pgrams {
float ADC128S102IO::convertVoltage(uint16_t value) {
  return LSB_ * value + halfLSB_;
}
float ADC128S102IO::getCurrentVoltage(int ch) {
  const uint8_t reg_value = (ch << 3) & (0xFF);
  writeBuffer_[0] = reg_value;
  writeBuffer_[1] = 0;
  const FT_STATUS status = spiInterface_->WriteAndRead(cs_, writeBuffer_, 2, readBuffer_);
  if (status != FT_OK) {
    std::cerr << "Error in getCurrentVoltage: " << status << std::endl;
    return status;
  }
  const uint16_t val = ((readBuffer_[0] & 0x00ff) << 8) | (readBuffer_[1] & 0xffff);
  return convertVoltage(val);
}
} // namespace gramsballoon::pgrams