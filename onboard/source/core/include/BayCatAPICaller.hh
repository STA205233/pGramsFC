#ifndef GB_BayCatAPICaller_hh
#define GB_BayCatAPICaller_hh 1

#include <atomic>
namespace gramsballoon::pgrams {

/**
 * @brief A class to manage the versalogic API. It ensures that the API is initialized once and also it provides wrapper functions for the API calls.
 * @author Shota Arai
 * @date 2026-04-10 | Shota Arai | Created
 * @date 2026-04-11 | Shota Arai | Added API wrapper functions
 * @date 2026-07-10 | Shota Arai | Added DIOGetChannelLevel
 * @ingroup device_core
 */
class BayCatAPICaller {
public:
  BayCatAPICaller();
  virtual ~BayCatAPICaller();
  /**
   * @brief Copy constructor, which increments the reference counter of the API
   */
  BayCatAPICaller(const BayCatAPICaller&);
  /**
   * @brief Return the status of the API. 0: not initialized, 1: initialized, <0: failed to initialize
   */
  int apiStatus() const { return apiStatus_.load(); }

protected:
  // SPI control
  /**
   * @brief Write one SPI data frame
   * @param[in] address Chip select address of the target device
   * @param[in] data Data frame to be written
   */
  int SPIWriteDataFrame(unsigned int address, uint32_t *data);
  /**
   * @brief Read one SPI data frame received in the last transaction
   * @param[out] data Buffer to store the read data frame
   */
  int SPIReadDataFrame(uint32_t *data);
  /**
   * @brief Set the SPI clock frequency
   * @param[in] frequency Frequency selector defined as SPI_CLK_FREQn_
   */
  int SPISetFrequency(unsigned int frequency);
  /**
   * @brief Set the SPI mode
   * @param[in] mode SPI mode (0-3)
   */
  int SPISetMode(unsigned int mode);
  /**
   * @brief Set the bit order of the SPI transfer
   * @param[in] direction SPI_DIR_LEFT_ for MSB first and SPI_DIR_RIGHT_ for LSB first
   */
  int SPISetShiftDirection(unsigned int direction);
  /**
   * @brief Set the size of one SPI data frame
   * @param[in] size Frame size in bits
   */
  int SPISetFrameSize(unsigned int size);
  /**
   * @brief Return whether the SPI bus is available
   */
  int SPIIsAvailable();

  // DIO control
  /**
   * @brief Get the direction of a DIO channel
   * @param[in] channel DIO channel number
   * @param[out] pDirection Buffer to store the direction
   */
  int DIOGetChannelDirection(unsigned char channel, unsigned char *pDirection);
  /**
   * @brief Set the direction of a DIO channel
   * @param[in] channel DIO channel number
   * @param[in] direction Direction to be set, e.g. DIO_OUTPUT_
   */
  void DIOSetChannelDirection(unsigned char channel, unsigned char direction);
  /**
   * @brief Set the output level of a DIO channel
   * @param[in] channel DIO channel number
   * @param[in] level Level to be set, DIO_CHANNEL_HIGH_ or DIO_CHANNEL_LOW_
   */
  void DIOSetChannelLevel(unsigned char channel, unsigned char level);
  /**
   * @brief Get the current level of a DIO channel
   * @param[in] channel DIO channel number
   */
  unsigned char DIOGetChannelLevel(unsigned char channel);

  // FPGA register access
  /**
   * @brief Read an FPGA register
   * @param[in] reg Register address
   * @param[out] data Buffer to store the read value
   */
  int FPGAReadRegister(unsigned long reg, unsigned char *data);
  /**
   * @brief Write an FPGA register
   * @param[in] reg Register address
   * @param[in] data Value to be written
   */
  int FPGAWriteRegister(unsigned long reg, unsigned char data);

  // I2C control
  /**
   * @brief Return whether the given I2C bus is available
   * @param[in] busNumber Bus number to be checked
   */
  int I2CIsAvailable(unsigned long busNumber);
  /**
   * @brief Read one register of an I2C device
   * @param[in] bType Bus type, e.g. I2C_BUS_TYPE_PRIMARY
   * @param[in] addr Address of the target device
   * @param[in] regNum Register number to be read
   * @param[out] data Buffer to store the read value
   */
  int I2CReadRegister(unsigned long bType, unsigned char addr,
                      unsigned short regNum, unsigned char *data);
  /**
   * @brief Read sequential bytes from an I2C device without specifying a register
   * @param[in] busType Bus type, e.g. I2C_BUS_TYPE_PRIMARY
   * @param[in] address Address of the target device
   * @param[out] data Buffer to store the read data
   * @param[in] numSequentialBytes Number of the bytes to be read
   */
  int I2CReadAddress(unsigned long busType, unsigned char address,
                     unsigned char *data, unsigned long numSequentialBytes);
  /**
   * @brief Write one register of an I2C device
   * @param[in] busType Bus type, e.g. I2C_BUS_TYPE_PRIMARY
   * @param[in] address Address of the target device
   * @param[in] registerNum Register number to be written
   * @param[in] data Value to be written
   */
  int I2CWriteRegister(unsigned long busType, unsigned char address,
                       unsigned short registerNum, unsigned char data);
  /**
   * @brief Write sequential bytes to an I2C device without specifying a register
   * @param[in] busType Bus type, e.g. I2C_BUS_TYPE_PRIMARY
   * @param[in] address Address of the target device
   * @param[in] pData Data to be written
   * @param[in] numSequentialBytes Number of the bytes to be written
   */
  int I2CWriteAddress(unsigned long busType, unsigned char address,
                      unsigned char *pData, unsigned long numSequentialBytes);
  /**
   * @brief Write to an I2C device and then read from it in one combined transaction
   * @param[in] busType Bus type, e.g. I2C_BUS_TYPE_PRIMARY
   * @param[in] address Address of the target device
   * @param[in] pWriteData Data to be written
   * @param[in] numWriteBytes Number of the bytes to be written
   * @param[out] pReadData Buffer to store the read data
   * @param[in] numReadBytes Number of the bytes to be read
   */
  int I2CWriteReadCombined(unsigned long busType, unsigned char address,
                           unsigned char *pWriteData, unsigned long numWriteBytes,
                           unsigned char *pReadData, unsigned long numReadBytes);
  /**
   * @brief Get the maximum clock frequency of an I2C bus
   * @param[in] busType Bus type, e.g. I2C_BUS_TYPE_PRIMARY
   * @param[out] data Buffer to store the frequency
   */
  int I2CGetMaxFrequency(unsigned long busType, unsigned long *data);
  /**
   * @brief Get the current clock frequency of an I2C bus
   * @param[in] busType Bus type, e.g. I2C_BUS_TYPE_PRIMARY
   * @param[out] data Buffer to store the frequency
   */
  int I2CGetFrequency(unsigned long busType, unsigned long *data);
  /**
   * @brief Set the clock frequency of an I2C bus
   * @param[in] busType Bus type, e.g. I2C_BUS_TYPE_PRIMARY
   * @param[in] data Frequency to be set, e.g. I2C_FREQUENCY_100KHZ
   */
  int I2CSetFrequency(unsigned long busType, unsigned long data);

  // Constants for VersaLogic API
  static const int SPI_CLK_FREQ0_;
  static const int SPI_CLK_FREQ1_;
  static const int SPI_CLK_FREQ2_;
  static const int SPI_CLK_FREQ3_;
  static const int SPI_DIR_LEFT_;
  static const int SPI_DIR_RIGHT_;
  static const int SPI_SS_SS0_;
  static const int DIO_OUTPUT_;
  static const int DIO_UNKNOWN_;
  static const int DIO_CHANNEL_HIGH_;
  static const int DIO_CHANNEL_LOW_;
  static const unsigned long I2C_BUS_TYPE_PRIMARY;
  static const unsigned long I2C_FREQUENCY_100KHZ;
  static const unsigned long I2C_FREQUENCY_400KHZ;

  static const int API_OK;

private:
  /**
   * @brief Initialize the VersaLogic API if it is not initialized yet
   */
  int startAPI();
  /**
   * @brief Close the VersaLogic API when no instance uses it any more
   */
  int stopAPI();

private:
  inline static std::atomic_int counter_{0};
  inline static std::atomic_int apiStatus_{0}; // 0: not initialized, 1: initialized, <0: failed to initialize
};
} // namespace gramsballoon::pgrams
#endif // GB_BayCatAPICaller_hh