#ifndef GRAMSBalloon_DAC121S101IO_hh
#define GRAMSBalloon_DAC121S101IO_hh 1
#include "SPIInterface.hh"
#include <cstdint>
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
  /**
   * @brief Constructor
   * @param[in] supply_voltage_in_V Supply voltage of the DAC in V, which gives the full scale of the output
   */
  DAC121S101IO(double supply_voltage_in_V = 3.3) : supplyVoltageInV_(supply_voltage_in_V) {}
  virtual ~DAC121S101IO() = default;

  /**
   * @brief Set the output voltage to the setting data held in this object, which is sent to the DAC by applySetting()
   * @param[in] voltage Output voltage in V
   * @return True if the voltage is within the range of the DAC
   */
  bool setVoltage(float voltage);
  /**
   * @brief Set the operation mode to the setting data held in this object, which is sent to the DAC by applySetting()
   * @param[in] mode Operation mode of the DAC
   */
  void setOperationMode(DAC121S101Mode mode);
  /**
   * @brief Send the setting data held in this object to the DAC via SPI
   * @return 0 on success, negative value on failure
   */
  int applySetting();
  /**
   * @brief Return the output voltage held in the setting data
   * @return Output voltage in V
   */
  float getCurrentVoltage() const;
  /**
   * @brief Convert a raw DAC value into a voltage in V
   * @param[in] value Raw DAC value
   * @return Voltage in V
   */
  float convertVoltage(uint16_t value) const;
  /**
   * @brief Set the SPI interface used to access the DAC
   * @param[in] spiInterface Pointer to the SPI interface
   */
  void setSPIInterface(SPIInterface *spiInterface) { spiInterface_ = spiInterface; }
  /**
   * @brief Return whether the SPI interface is already set
   * @return True if the SPI interface is set
   */
  bool isSPIInterfaceSet() const { return spiInterface_ != nullptr; }
  /**
   * @brief Set the chip select channel of the DAC
   * @param[in] cs Chip select channel
   */
  void setCS(int cs) { cs_ = cs; }
  /**
   * @brief Return the chip select channel of the DAC
   * @return Chip select channel
   */
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
  /**
   * @brief Extract the operation mode bits from the setting data
   * @return Operation mode bits
   */
  uint8_t extractOperationModeBit() {
    return (settingData_[0] >> 4) & MODE_MASK_IN_UPPER_BYTE;
  }
  /**
   * @brief Extract the data bits from the setting data
   * @return Raw DAC value
   */
  uint16_t extractDataBit() const {
    return (static_cast<uint16_t>(settingData_[0] & DATABIT_MASK_IN_UPPER_BYTE) << 8) | static_cast<uint16_t>(settingData_[1] & DATABIT_MASK_IN_LOWER_BYTE);
  }

  /**
   * @brief Return the full scale of the output voltage
   * @return Maximum output voltage in V
   */
  float getMaximumVoltage() const {
    return convertVoltage(0x1000U);
  }
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_DAC121S101IO_hh