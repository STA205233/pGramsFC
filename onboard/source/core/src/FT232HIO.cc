#include "FT232HIO.hh"
#include <iostream>
namespace gramsballoon::pgrams {
FT232HIO::FT232HIO() {
  mpsseDeviceManager_ = std::make_shared<mpsse::MPSSEDeviceManager>();
  mpsseController_ = std::make_shared<mpsse::MPSSEController>();
}
int FT232HIO::Open(int channel) {
  int status = mpsseDeviceManager_->openDevice(channel, *mpsseController_);
  if (status != 0) {
    std::cerr << "FT232HIO::Open: Failed to open device at channel " << channel << std::endl;
    return status;
  }
  status = mpsseController_->initialize();
  if (status != 0) {
    std::cerr << "FT232HIO::Open: Failed to initialize MPSSE controller" << std::endl;
    return status;
  }
  setIsOpen(true);
  return 0;
}

int FT232HIO::WriteThenRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) {
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
  int num_transfered = WriteAndRead(cs, &writeBuffer_[0], wsize + rsize, &readBuffer_[0]);
  if (num_transfered < 0) {
    std::cerr << "SPI_ReadWrite: WriteAndRead failed" << std::endl;
    setIsOpen(false);
    return num_transfered;
  }
  if (num_transfered != (wsize + rsize)) {
    std::cerr << "SPI_ReadWrite: Not all bytes were written" << std::endl;
    setIsOpen(false);
    return -static_cast<int>(FT_OTHER_ERROR);
  }
  for (int i = 0; i < rsize; ++i) {
    readBuffer[i] = readBuffer_[i + wsize];
  }
  return 0;
}
int FT232HIO::WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer) {
  int num_transfered = mpsseController_->write_readSPI(writeBuffer, size, readBuffer, cs);
  if (num_transfered != static_cast<int>(size)) {
    std::cerr << "SPI_ReadWrite: Not all bytes were written" << std::endl;
    setIsOpen(false);
    return -static_cast<int>(FT_OTHER_ERROR);
  }
  return num_transfered;
}
int FT232HIO::Write(int cs, const uint8_t *writeBuffer, unsigned int size) {
  writeBuffer_.clear();
  for (int i = 0; i < static_cast<int>(size); ++i) {
    writeBuffer_.push_back(writeBuffer[i]);
  }
  const int num_transfered = mpsseController_->writeSPI(&writeBuffer_[0], size, cs);
  if (num_transfered != static_cast<int>(size)) {
    std::cerr << "SPI_Write: Not all bytes were written" << std::endl;
    setIsOpen(false);
    return -static_cast<int>(FT_OTHER_ERROR);
  }
  return num_transfered;
}
int FT232HIO::controlGPIO(int cs, bool value) {
  if (mpsseController_->writeGPIO(cs, value) != 0) {
    setIsOpen(false);
    return -1;
  }
  return 0;
}
int FT232HIO::controlGPIOMul(const uint32_t state) {
  if (mpsseController_->writeGPIOMul(state) != 0) {
    setIsOpen(false);
    return -1;
  }
  return 0;
}
int FT232HIO::updateSetting() {
  const unsigned int baudrate = Baudrate();
  const unsigned int configOptions = ConfigOptions();
  if (mpsseController_) {
    mpsseController_->setSPIMode(configOptions & 0x3);
    mpsseController_->setBaudrate(baudrate);
    if (mpsseController_->applySettings() != 0) {
      std::cerr << "FT232HIO::updateSetting: Failed to apply settings" << std::endl;
      setIsOpen(false);
      return -1;
    }
    return 0;
  }
  else {
    setIsOpen(false);
    return -1;
  }
}
} // namespace gramsballoon::pgrams