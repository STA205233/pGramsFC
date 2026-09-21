#ifndef GRAMSBalloon_ADC128S102IO_HH
#define GRAMSBalloon_ADC128S102IO_HH 1
#include "SPIInterface.hh"
#include <array>
#include <cstdint>
namespace gramsballoon::pgrams {
/**
 * @brief A class to handle the ADC128S102, an 8-channel 12-bit ADC read out via SPI
 * @author Shota Arai
 * @date 2025-**-** | Shota Arai | First version
 * @ingroup device_core
 */
class ADC128S102IO {
public:
  /**
   * @brief Constructor
   * @param[in] supply_voltage_in_V Supply voltage of the ADC in V, used to calculate the LSB
   */
  ADC128S102IO(float supply_voltage_in_V = 3.3) : LSB_(supply_voltage_in_V / 4096), halfLSB_(LSB_ * 0.5) {}
  virtual ~ADC128S102IO() = default;
  /**
   * @brief Return the number of the analog input channels of one ADC chip
   */
  static constexpr size_t MaxChannelsPerADC() { return 8; }

private:
  SPIInterface *spiInterface_ = nullptr;
  int cs_ = 0;
  float LSB_ = 3.3 / 4096;
  float halfLSB_ = LSB_ * 0.5;
  uint8_t readBuffer_[2] = {0, 0};
  uint8_t writeBuffer_[2] = {0, 0};
  int errorCode_ = 0;

public:
  /**
   * @brief Read one channel and return the voltage in V
   * @param[in] ch Channel number to be read
   */
  float getCurrentVoltage(int ch);
  /**
   * @brief Read one channel and return the raw ADC value
   * @param[in] ch Channel number to be read
   */
  uint16_t getCurrentVoltageADC(int ch);
  /**
   * @brief Read all the channels and store the voltages in V
   * @param[out] dest Destination array of the voltages
   */
  bool getAllADCs(std::array<float, 8> &dest);
  /**
   * @brief Read all the channels and store the raw ADC values
   * @param[out] dest Destination array of the ADC values
   */
  bool getAllADCs(std::array<uint16_t, 8> &dest);
  /**
   * @brief Convert a raw ADC value into a voltage in V
   * @param[in] value Raw ADC value
   */
  float convertVoltage(uint16_t value) const;
  /**
   * @brief Return the error code of the last SPI access
   */
  int getErrorCode() const { return errorCode_; }
  /**
   * @brief Return true if an error occurred in the last SPI access
   */
  bool isError() const { return errorCode_ != 0; }
  /**
   * @brief Clear the error code
   */
  void resetError() { errorCode_ = 0; }
  /**
   * @brief Set the SPI interface used to access the ADC
   * @param[in] spiInterface Pointer to the SPI interface
   */
  void setSPIInterface(SPIInterface *spiInterface) { spiInterface_ = spiInterface; }
  /**
   * @brief Return true if the SPI interface is already set
   */
  bool isSPIInterfaceSet() const { return spiInterface_ != nullptr; }
  /**
   * @brief Set the chip select channel of the ADC
   * @param[in] cs Chip select channel
   */
  void setCS(int cs) { cs_ = cs; }
  /**
   * @brief Return the chip select channel of the ADC
   */
  int getCS() const { return cs_; }
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_ADC128S102IO_HH