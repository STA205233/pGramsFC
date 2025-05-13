#include "SPIInterface.hh"
#include <iostream>
namespace gramsballoon::pgrams {
FT_STATUS SPIInterface::Open(int channel) {
  DWORD numChannels_ = 0;
  {
    const FT_STATUS status = SPI_GetNumChannels(&numChannels_);
    if (status != FT_OK) {
      std::cerr << "SPI_GetNumChannels failed: status = " << status << std::endl;
      return status;
    }
    if (numChannels_ == 0) {
      std::cerr << "No SPI channels found" << std::endl;
      return FT_DEVICE_NOT_FOUND;
    }
    else if (numChannels_ <= static_cast<DWORD>(channel)) {
      std::cerr << "Invalid SPI channel: " << channel << std::endl;
      return FT_INVALID_PARAMETER;
    }
  }
  {
    const FT_STATUS status = SPI_OpenChannel(channel, &SPIHandler_);
    if (status != FT_OK) {
      std::cerr << "SPI_OpenChannel failed: status = " << status << std::endl;
      return status;
    }
  }
  {
    channelConfig_.LatencyTimer = 25;
    const FT_STATUS status = SPI_InitChannel(SPIHandler_, &channelConfig_);
    if (status != FT_OK) {
      std::cerr << "SPI_InitChannel failed: status = " << status << std::endl;
      return status;
    }
  }
  return FT_OK;
}

FT_STATUS SPIInterface::WriteAfterRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) {
  writeBuffer_.clear();
  readBuffer_.clear();
  for (int i = 0; i < wsize; ++i) {
    writeBuffer_.push_back(writeBuffer[i]);
    readBuffer_.push_back(0);
  }
  for (int i = 0; i < rsize; ++i) {
    writeBuffer_.push_back(0);
    readBuffer_.push_back(0);
  }
  DWORD num_transfered;
  if (cs == SPI_INTERNAL_CS) {
    const FT_STATUS status = SPI_ReadWrite(SPIHandler_, &readBuffer_[0], &writeBuffer_[0], wsize + rsize, &num_transfered, TRANSFER_OPTION_INTERNAL_CS);
    if (status != FT_OK) {
      std::cerr << "SPI_ReadWrite failed: status = " << status << std::endl;
      return status;
    }
  }
  else {
    int cs_ = 1 << static_cast<uint8_t>(cs);
    std::cout << "SPIInterface::WriteAndRead: cs_ = " << cs_ << std::endl;
    const FT_STATUS status_cs = FT_WriteGPIO(SPIHandler_, cs_, 0);
    if (status_cs != FT_OK) {
      std::cerr << "FT_WriteGPIO failed: status = " << status_cs << std::endl;
      return status_cs;
    }
    const FT_STATUS status = SPI_ReadWrite(SPIHandler_, &readBuffer_[0], &writeBuffer_[0], wsize + rsize, &num_transfered, TRANSFER_OPTION_EXTERNAL_CS);
    const FT_STATUS status_cs2 = FT_WriteGPIO(SPIHandler_, cs_, 1);
    if (status_cs2 != FT_OK) {
      std::cerr << "FT_WriteGPIO failed: status = " << status_cs2 << std::endl;
      return status_cs2;
    }
    if (status != FT_OK) {
      std::cerr << "SPI_ReadWrite failed: status = " << status << std::endl;
      return status;
    }
  }
  if (num_transfered != static_cast<DWORD>(wsize + rsize)) {
    std::cerr << "SPI_ReadWrite: Not all bytes were written" << std::endl;
    return FT_OTHER_ERROR;
  }
  for (int i = 0; i < rsize; ++i) {
    readBuffer[i] = readBuffer_[i + wsize];
  }
  return FT_OK;
}
FT_STATUS SPIInterface::WriteAndRead(int cs, const uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer) {
  unsigned int num_transfered;
  if (cs == SPI_INTERNAL_CS) {
    const FT_STATUS status = SPI_ReadWrite(SPIHandler_, &readBuffer_[0], &writeBuffer_[0], size, &num_transfered, TRANSFER_OPTION_EXTERNAL_CS);
    if (status != FT_OK) {
      std::cerr << "SPI_ReadWrite failed: status = " << status << std::endl;
      return status;
    }
  }
  else {
    const FT_STATUS status_cs = FT_WriteGPIO(SPIHandler_, cs, 0);
    if (status_cs != FT_OK) {
      std::cerr << "FT_WriteGPIO failed: status = " << status_cs << std::endl;
      return status_cs;
    }
    const FT_STATUS status = SPI_ReadWrite(SPIHandler_, &readBuffer_[0], &writeBuffer_[0], size, &num_transfered, TRANSFER_OPTION_EXTERNAL_CS);
    const FT_STATUS status_cs2 = FT_WriteGPIO(SPIHandler_, cs, 1);
    if (status_cs2 != FT_OK) {
      std::cerr << "FT_WriteGPIO failed: status = " << status_cs2 << std::endl;
      return status_cs2;
    }
    if (status != FT_OK) {
      std::cerr << "SPI_ReadWrite failed: status = " << status << std::endl;
      return status;
    }
  }
  if (num_transfered != size) {
    std::cerr << "SPI_ReadWrite: Not all bytes were written" << std::endl;
    return FT_OTHER_ERROR;
  }
  return FT_OK;
}
FT_STATUS SPIInterface::Write(int cs, const uint8_t *writeBuffer, unsigned int size) {
  writeBuffer_.clear();
  for (int i = 0; i < size; ++i) {
    writeBuffer_.push_back(writeBuffer[i]);
  }
  std::cout << "SPIInterface::Write: writeBuffer = ";
  for (const auto &i: writeBuffer_) {
    std::cout << std::hex << static_cast<int>(i) << " ";
  }
  std::cout << std::endl;
  DWORD num_transfered;
  if (cs == SPI_INTERNAL_CS) {
    const FT_STATUS status = SPI_Write(SPIHandler_, &writeBuffer_[0], size, &num_transfered, TRANSFER_OPTION_INTERNAL_CS);
    if (status != FT_OK) {
      std::cerr << "SPI_Write failed: status = " << status << std::endl;
      return status;
    }
  }
  else {
    int cs_ = 1 << static_cast<uint8_t>(cs);
    const FT_STATUS status_cs = FT_WriteGPIO(SPIHandler_, cs_, 0);
    if (status_cs != FT_OK) {
      std::cerr << "FT_WriteGPIO failed: status = " << status_cs << std::endl;
      return status_cs;
    }
    const FT_STATUS status = SPI_Write(SPIHandler_, &writeBuffer_[0], size, &num_transfered, TRANSFER_OPTION_EXTERNAL_CS);
    const FT_STATUS status_cs2 = FT_WriteGPIO(SPIHandler_, cs_, 1);
    if (status_cs2 != FT_OK) {
      std::cerr << "FT_WriteGPIO failed: status = " << status_cs2 << std::endl;
      return status_cs2;
    }
    if (status != FT_OK) {
      std::cerr << "SPI_Write failed: status = " << status << std::endl;
      return status;
    }
  }
  if (num_transfered != static_cast<DWORD>(size)) {
    std::cerr << "SPI_Write: Not all bytes were written" << std::endl;
    return FT_OTHER_ERROR;
  }
  return FT_OK;
}
} // namespace gramsballoon::pgrams