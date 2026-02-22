#ifndef GRAMSBalloon_DAC121S101IO_hh
#define GRAMSBalloon_DAC121S101IO_hh 1
#include "SPIInterface.hh"
#include "cstdint"
#include <memory>
namespace gramsballoon::pgrams {
/**
 * @brief DAC121S101IO class for handling DAC121S101
 *
 * This class provides an interface to control the DAC121S101 digital-to-analog converter.
 * It allows setting the output voltage and reading the current output voltage.
 *
 * @author Shota Arai
 * @date 2025-05-12 | first draft
 */
enum class DAC121S101Mode : uint8_t {
  DAC121S101_MODE_NORMAL = 0,
  DAC121S101_MODE_PowerDown_1kohm = 1,
  DAC121S101_MODE_PowerDown_100kohm = 2,
  DAC121S101_MODE_PowerDown_HiZ = 3,
};
class DAC121S101IO {
public:
  DAC121S101IO(double supply_voltage_in_V = 3.3) : supplyVoltageInV_(supply_voltage_in_V) {}
  virtual ~DAC121S101IO() = default;

  void setVoltage(float voltage);
  void setOperationMode(DAC121S101Mode mode);
  int applySetting();
  float getCurrentVoltage() const;
  float convertVoltage(uint16_t value) const;
  void setSPIInterface(SPIInterface *spiInterface) { spiInterface_ = spiInterface; }
  bool isSPIInterfaceSet() const { return spiInterface_ != nullptr; }
  void setCS(int cs) { cs_ = cs; }
  int getCS() const { return cs_; }

private:
  double supplyVoltageInV_ = 3.3;
  static constexpr uint8_t DATABIT_MASK_IN_LOWER_BYTE = 0xff;
  static constexpr uint8_t DATABIT_MASK_IN_UPPER_BYTE = 0x0f;
  static constexpr uint8_t MODE_MASK_IN_UPPER_BYTE = 0x30;
  static constexpr uint16_t MODE_MASK = MODE_MASK_IN_UPPER_BYTE << 4;
  static constexpr uint16_t DATABIT_MASK = (static_cast<uint16_t>(DATABIT_MASK_IN_UPPER_BYTE) << 8) | static_cast<uint16_t>(DATABIT_MASK_IN_LOWER_BYTE);
  int cs_ = 0; // Chip Select
  SPIInterface *spiInterface_ = nullptr;
  uint8_t settingData_[2] = {0, 0}; // 16 bits
  uint8_t extractOperationModeBit() {
    return (settingData_[0] >> 4) & MODE_MASK_IN_UPPER_BYTE;
  }
  uint16_t extractDataBit() const {
    return (static_cast<uint16_t>(settingData_[0] & DATABIT_MASK_IN_UPPER_BYTE) << 8) | static_cast<uint16_t>(settingData_[1] & DATABIT_MASK_IN_LOWER_BYTE);
  }
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_DAC121S101IO_hh