#ifndef GB_BayCatAPICaller_hh
#define GB_BayCatAPICaller_hh 1

#include <atomic>
namespace gramsballoon::pgrams {

/**
 * @brief A class to manage the versalogic API. It ensures that the API is initialized once and also it provides wrapper functions for the API calls.
 * @author Shota Arai
 * @date 2026-04-10 | Shota Arai | Created
 * @date 2026-04-11 | Shota Arai | Added API wrapper functions
 */
class BayCatAPICaller {
public:
  BayCatAPICaller();
  virtual ~BayCatAPICaller();
  BayCatAPICaller(const BayCatAPICaller &);
  int apiStatus() const { return apiStatus_.load(); }

protected:
  // SPI control
  int SPIWriteDataFrame(unsigned int address, uint32_t *data);
  int SPIReadDataFrame(uint32_t *data);
  int SPISetFrequency(unsigned int frequency);
  int SPISetMode(unsigned int mode);
  int SPISetShiftDirection(unsigned int direction);
  int SPISetFrameSize(unsigned int size);
  int SPIIsAvailable();

  // DIO control
  int DIOGetChannelDirection(unsigned char channel, unsigned char *pDirection);
  void DIOSetChannelDirection(unsigned char channel, unsigned char direction);
  void DIOSetChannelLevel(unsigned char channel, unsigned char level);

  // FPGA register access
  int FPGAReadRegister(unsigned long reg, unsigned char *data);
  int FPGAWriteRegister(unsigned long reg, unsigned char data);

  // I2C control
  int I2CIsAvailable(unsigned long busNumber);
  int I2CReadRegister(unsigned long bType, unsigned char addr,
                      unsigned short regNum, unsigned char *data);
  int I2CReadAddress(unsigned long busType, unsigned char address,
                     unsigned char *data, unsigned long numSequentialBytes);
  int I2CWriteRegister(unsigned long busType, unsigned char address,
                       unsigned short registerNum, unsigned char data);
  int I2CWriteAddress(unsigned long busType, unsigned char address,
                      unsigned char *pData, unsigned long numSequentialBytes);
  int I2CWriteReadCombined(unsigned long busType, unsigned char address,
                           unsigned char *pWriteData, unsigned long numWriteBytes,
                           unsigned char *pReadData, unsigned long numReadBytes);
  int I2CGetMaxFrequency(unsigned long busType, unsigned long *data);
  int I2CGetFrequency(unsigned long busType, unsigned long *data);
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
  static const int DIO_CHANNEL_HIGH_;
  static const int DIO_CHANNEL_LOW_;
  static const unsigned long I2C_BUS_TYPE_PRIMARY;

  static const int API_OK;

private:
  int startAPI();
  int stopAPI();

private:
  inline static std::atomic_int counter_{0};
  inline static std::atomic_int apiStatus_{0}; // 0: not initialized, 1: initialized, <0: failed to initialize
};
} // namespace gramsballoon::pgrams
#endif //GB_BayCatAPICaller_hh