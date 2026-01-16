#include "DAC121S101IO.hh"
#include "ADC128S102IO.hh"
namespace gramsballoon::pgrams {
void DAC121S101IO::setVoltage(float voltage) {
  // Convert voltage to 12-bit data
  uint16_t data = static_cast<uint16_t>((voltage / supplyVoltageInV_ * 4096) * DATABIT_MASK);
  settingData_[1] = (settingData_[1] & ~DATABIT_MASK_IN_LOWER_BYTE) | (data & DATABIT_MASK);
  settingData_[0] = (settingData_[0] & ~DATABIT_MASK_IN_UPPER_BYTE) | ((data & DATABIT_MASK) >> 8);
}
void DAC121S101IO::setOperationMode(DAC121S101Mode mode) {
  settingData_[0] = (settingData_[0] & ~MODE_MASK_IN_UPPER_BYTE) | (static_cast<uint8_t>(mode) << 4);
}
int DAC121S101IO::applySetting() {
  if (spiInterface_) {
    const int status = spiInterface_->Write(cs_, settingData_, sizeof(settingData_));
    if (status < 0) {
      std::cerr << "DAC1215101IO::applySetting: Cannot write register. Code " << status << std::endl;
      return -1;
    }
    return 0;
  }
  return -1;
}
float DAC121S101IO::getCurrentVoltage() const {
  return (static_cast<float>(settingData_[0] & DATABIT_MASK_IN_UPPER_BYTE) / DATABIT_MASK);
}
float DAC121S101IO::convertVoltage(uint16_t value) const {
  return value * supplyVoltageInV_ / 4096;
}
} // namespace gramsballoon::pgrams