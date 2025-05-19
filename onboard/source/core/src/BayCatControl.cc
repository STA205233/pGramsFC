#include "BayCatControl.hh"
namespace gramsballoon::pgrams {
BayCatControl::BayCatControl() {
  baudrateList_.emplace(750000, SPI_CLK_FREQ0);
  baudrateList_.emplace(1500000, SPI_CLK_FREQ1);
  baudrateList_.emplace(2000000, SPI_CLK_FREQ2);
  baudrateList_.emplace(6000000, SPI_CLK_FREQ3);
}
void BayCatControl::setBaudrate(unsigned int baudrate) {
  if (!baudrateList_.count(baudrate) == 0) {
    std::cerr << "Baudrate " << baudrate << " Hz is not working." << std::endl;
    return;
  }
  baudrate_ = baudrate;
  const VL_APIStatusT status = VSL_SPISetFrequency(baudrateList_[baudrate_]);
  if (status != VL_API_OK) {
    std::cerr << "SPISetFrequency failed: " << status << std::endl;
  }
}
int BayCatControl::updateSetting() {
  if (!isOpen_) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  bool failed = false;
  const VL_APIStatusT status = VSL_SPISetMode(options_ & SPI_MODE_MASK);
  if (status != VL_API_OK) {
    std::cerr << "SPISetMode failed: " << status << std::endl;
    failed = true;
  }
  const VL_APIStatusT status2 = VSL_SPISetShiftDirection(options_ & SPI_SHIFT_DIRECTION);
  if (status2 != VL_API_OK) {
    std::cerr << "SPISetShiftDiretcion failed: " << status << std::endl;
    failed = true;
  }
  const VL_APIStatusT status3 = VSL_SPISetFrameSize(1);
  if (status3 != VL_API_OK) {
    std::cerr << "SPISetFrameSize failed: " << status << std::endl;
    failed = true;
  }
  if (failed) return -1;
  return 0;
}
int BayCatControl::Open(int) {
  if (isOpen_) {
    return 0;
  }
  const auto status = VSL_Open();
  if (status != 0) {
    std::cerr << "VSL_Open failed: " << status << std::endl;
    return status;
  }
  isOpen_ = true;
  return status;
}
int BayCatControl::Close() {
  if (!isOpen_) {
    return 0;
  }
  const auto status = VSL_Close();
  if (status != 0) {
    std::cerr << "VSL_Close failed: " << status << std::endl;
    return status;
  }
  isOpen_ = false;
  return status;
}
int BayCatControl::WriteAfterRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) {
  if (!isOpen_) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (wsize <= 0 || rsize <= 0) {
    std::cerr << "Invalid size: wsize = " << wsize << ", rsize = " << rsize << std::endl;
    return -1;
  }
  const auto status_cs_low = controlGPIO(cs, false);
  if (status_cs_low != 0) {
    std::cerr << "controlGPIO failed: " << status_cs_low << std::endl;
    return -1;
  }
  uint32_t write_data = 0;
  for (int i = 0; i < wsize; ++i) {
    write_data = writeBuffer[i];
    const auto status_write = VSL_SPIWriteDataFrame(SPI_SS_SS0, &write_data); // assuming not using VL_SPI_SS0
    if (status_write != VL_API_OK) {
      std::cerr << "SPIWriteDataFrame failed: " << status_write << std::endl;
      return -1;
    }
  }
  uint32_t read_data = 0;
  for (int i = 0; i < rsize; ++i) {
    const auto status_read = VSL_SPIReadDataFrame(&read_data); // assuming not using VL_SPI_SS0
    if (status_read != VL_API_OK) {
      std::cerr << "SPIReadDataFrame failed: " << status_read << std::endl;
      return -1;
    }
    readBuffer[i] = static_cast<uint8_t>(read_data);
  }
  const auto status_cs_high = controlGPIO(cs, true);
  if (status_cs_high != 0) {
    std::cerr << "controlGPIO failed: " << status_cs_high << std::endl;
    return -1;
  }
  return 0;
}
int BayCatControl::controlGPIO(int cs, bool value) {
  unsigned char direction = 0;
  const auto status = VSL_DIOGetChannelDirection(cs, &direction);
  if (status != VL_API_OK) {
    std::cerr << "VSL_DIOGetChannelDirection failed: " << status << std::endl;
    return -1;
  }
  if (direction != DIO_OUTPUT) {
    std::cerr << "Channel " << cs << " is not set to output" << std::endl;
    return -2;
  }
  unsigned char value_ = 0;
  if (value) {
    value_ = DIO_CHANNEL_HIGH;
  }
  else {
    value_ = DIO_CHANNEL_LOW;
  }
  VSL_DIOSetChannelLevel(cs, value_);
  return 0;
}
int BayCatControl::Write(int cs, const uint8_t *writeBuffer, unsigned int size) {
  if (!isOpen_) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (size <= 0) {
    std::cerr << "Invalid size: size = " << size << std::endl;
    return -1;
  }
  const auto status_cs_low = controlGPIO(cs, false);
  if (status_cs_low != 0) {
    std::cerr << "controlGPIO failed: " << status_cs_low << std::endl;
    return -1;
  }
  uint32_t write_data = 0;
  for (unsigned int i = 0; i < size; ++i) {
    write_data = writeBuffer[i];
    const auto status_write = VSL_SPIWriteDataFrame(SPI_SS_SS0, &write_data); // assuming not using VL_SPI_SS0
    if (status_write != VL_API_OK) {
      std::cerr << "SPIWriteDataFrame failed: " << status_write << std::endl;
      return -1;
    }
  }
  const auto status_cs_high = controlGPIO(cs, true);
  if (status_cs_high != 0) {
    std::cerr << "controlGPIO failed: " << status_cs_high << std::endl;
    return -1;
  }
  return 0;
}
} // namespace gramsballoon::pgrams