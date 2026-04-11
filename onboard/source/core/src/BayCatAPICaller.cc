#include "BayCatAPICaller.hh"
// clang-format off
#define linux 1
#include "stdbool.h"
#include "VL_OSALib.h"
// clang-format on
#include <iostream>
namespace gramsballoon::pgrams {
const int BayCatAPICaller::SPI_CLK_FREQ0_ = SPI_CLK_FREQ0;
const int BayCatAPICaller::SPI_CLK_FREQ1_ = SPI_CLK_FREQ1;
const int BayCatAPICaller::SPI_CLK_FREQ2_ = SPI_CLK_FREQ2;
const int BayCatAPICaller::SPI_CLK_FREQ3_ = SPI_CLK_FREQ3;
const int BayCatAPICaller::SPI_DIR_LEFT_ = SPI_DIR_LEFT;
const int BayCatAPICaller::SPI_DIR_RIGHT_ = SPI_DIR_RIGHT;
const int BayCatAPICaller::SPI_SS_SS0_ = SPI_SS_SS0;
const int BayCatAPICaller::DIO_OUTPUT_ = DIO_OUTPUT;
const int BayCatAPICaller::DIO_CHANNEL_HIGH_ = DIO_CHANNEL_HIGH;
const int BayCatAPICaller::DIO_CHANNEL_LOW_ = DIO_CHANNEL_LOW;
const int BayCatAPICaller::API_OK = VL_API_OK;
const unsigned long BayCatAPICaller::I2C_BUS_TYPE_PRIMARY = VL_I2C_BUS_TYPE_PRIMARY;
BayCatAPICaller::BayCatAPICaller() {
  if (++counter_ == 0) {
    startAPI();
  }
}
BayCatAPICaller::~BayCatAPICaller() {
  if (--counter_ == 0) {
    stopAPI();
  }
}
BayCatAPICaller::BayCatAPICaller(const BayCatAPICaller &) {
  if (++counter_ == 0) {
    startAPI();
  }
}
int BayCatAPICaller::startAPI() {
  std::cout << "Initializing VersaLogic Library..." << std::endl;
  const auto ret = VL_Open();
  if (ret != 0) {
    std::cerr << "VL_Open failed: " << ret << std::endl;
    apiStatus_.store(ret);
  }
  else {
    apiStatus_.store(1);
  }
  return ret;
}
int BayCatAPICaller::stopAPI() {
  std::cout << "Closing VersaLogic Library..." << std::endl;
  const auto ret = VL_Close();
  apiStatus_.store(0);
  return ret;
}

// API wrapper functions
int BayCatAPICaller::SPIWriteDataFrame(unsigned int address, uint32_t *data) {
  return VSL_SPIWriteDataFrame(address, data);
}
int BayCatAPICaller::SPIReadDataFrame(uint32_t *data) {
  return VSL_SPIReadDataFrame(data);
}
int BayCatAPICaller::SPISetFrequency(unsigned int frequency) {
  return VSL_SPISetFrequency(frequency);
}
int BayCatAPICaller::SPISetMode(unsigned int mode) {
  return VSL_SPISetMode(mode);
}
int BayCatAPICaller::SPISetShiftDirection(unsigned int direction) {
  return VSL_SPISetShiftDirection(direction);
}
int BayCatAPICaller::SPISetFrameSize(unsigned int size) {
  return VSL_SPISetFrameSize(size);
}
int BayCatAPICaller::SPIIsAvailable() {
  return VSL_SPIIsAvailable();
}

int BayCatAPICaller::DIOGetChannelDirection(unsigned char channel, unsigned char *pDirection) {
  return VSL_DIOGetChannelDirection(channel, pDirection);
}
void BayCatAPICaller::DIOSetChannelDirection(unsigned char channel, unsigned char direction) {
  VSL_DIOSetChannelDirection(channel, direction);
}
void BayCatAPICaller::DIOSetChannelLevel(unsigned char channel, unsigned char level) {
  VSL_DIOSetChannelLevel(channel, level);
}
int BayCatAPICaller::FPGAReadRegister(unsigned long reg, unsigned char *data) {
  return VSL_FPGAReadRegister(reg, data);
}
int BayCatAPICaller::FPGAWriteRegister(unsigned long reg, unsigned char data) {
  return VSL_FPGAWriteRegister(reg, data);
}

int BayCatAPICaller::I2CIsAvailable(unsigned long busNumber) {
  return VSL_I2CIsAvailable(busNumber);
}
int BayCatAPICaller::I2CReadRegister(unsigned long busType, unsigned char addr,
                                     unsigned short regNum, unsigned char *data) {
  return VSL_I2CReadRegister(busType, addr, regNum, data);
}
int BayCatAPICaller::I2CReadAddress(unsigned long busType, unsigned char address,
                                    unsigned char *data, unsigned long numSequentialBytes) {
  return VSL_I2CReadAddress(busType, address, data, numSequentialBytes);
}
int BayCatAPICaller::I2CWriteRegister(unsigned long busType, unsigned char address,
                                      unsigned short registerNum, unsigned char data) {
  return VSL_I2CWriteRegister(busType, address, registerNum, data);
}
int BayCatAPICaller::I2CWriteAddress(unsigned long busType, unsigned char address,
                                     unsigned char *pData, unsigned long numSequentialBytes) {
  return VSL_I2CWriteAddress(busType, address, pData, numSequentialBytes);
}
int BayCatAPICaller::I2CWriteReadCombined(unsigned long busType, unsigned char address,
                                          unsigned char *pWriteData, unsigned long numWriteBytes,
                                          unsigned char *pReadData, unsigned long numReadBytes) {
  return VSL_I2CWriteReadCombined(busType, address, pWriteData, numWriteBytes, pReadData, numReadBytes);
}
int BayCatAPICaller::I2CGetMaxFrequency(unsigned long busType, unsigned long *data) {
  return VSL_I2CGetMaxFrequency(busType, data);
}
int BayCatAPICaller::I2CGetFrequency(unsigned long busType, unsigned long *data) {
  return VSL_I2CGetFrequency(busType, data);
}
int BayCatAPICaller::I2CSetFrequency(unsigned long busType, unsigned long data) {
  return VSL_I2CSetFrequency(busType, data);
}

} // namespace gramsballoon::pgrams
