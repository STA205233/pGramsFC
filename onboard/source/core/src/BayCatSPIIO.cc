#include "BayCatSPIIO.hh"
#define DEBUG_SPI 1
#ifdef DEBUG_SPI
#include <iomanip>
#endif
namespace gramsballoon::pgrams {
BayCatSPIIO::BayCatSPIIO() : SPIInterface(), BayCatAPICaller() {
  baudrateList_.emplace(100000, SPI_CLK_FREQ0_);
  baudrateList_.emplace(200000, SPI_CLK_FREQ1_);
  baudrateList_.emplace(400000, SPI_CLK_FREQ2_);
  baudrateList_.emplace(800000, SPI_CLK_FREQ3_);
}
void BayCatSPIIO::setBaudrate(unsigned int baudrate) {
  if (baudrateList_.count(baudrate) == 0) {
    std::cerr << "Baudrate " << baudrate << " Hz is not working." << std::endl;
    return;
  }
  SPIInterface::setBaudrate(baudrate);
}
int BayCatSPIIO::applyBaudrateSetting() {
  const int status = SPISetFrequency(baudrateList_[Baudrate()]);
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: baudrate_" << Baudrate() << std::endl;
  std::cout << "BayCatSPIIO: baudrateList_[baudrate_]: " << baudrateList_[Baudrate()] << std::endl; //For Debug
#endif
  if (status != API_OK) {
    std::cerr << "SPISetFrequency failed: " << status << std::endl;
    return static_cast<int>(status);
  }
  return 0;
}
int BayCatSPIIO::updateSetting() {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  const auto options = ConfigOptions();
  bool failed = false;
  const int status = SPISetMode(options & SPI_MODE_MASK);
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: options_ & SPI_MODE_MASK: " << (options & SPI_MODE_MASK) << std::endl; //For Debug
#endif
  if (status != API_OK) {
    std::cerr << "SPISetMode failed: " << status << std::endl;
    failed = true;
  }

  const unsigned int shift_direction = (options & SPI_SHIFT_DIRECTION_MASK) >> SPI_SHIFT_DIRECTION_OFFSET;
  if (shift_direction != SPI_DIR_LEFT_ && shift_direction != SPI_DIR_RIGHT_) {
    std::cerr << "ShiftDirection is invalid: " << SPI_DIR_RIGHT_ << " or " << SPI_DIR_LEFT_ << " are allowed.";
    failed = false;
  }
  else {
    const int status2 = SPISetShiftDirection(shift_direction);
#ifdef DEBUG_SPI
    std::cout << "BayCatSPIIO: shift_direction: " << shift_direction << std::endl; //For Debug
#endif
    if (status2 != API_OK) {
      std::cerr << "SPISetShiftDiretcion failed: " << status << std::endl;
      failed = true;
    }
  }
  const int status3 = SPISetFrameSize(1);
  if (status3 != API_OK) {
    std::cerr << "SPISetFrameSize failed: " << status << std::endl;
    failed = true;
  }
  const int status4 = applyBaudrateSetting();
  if (status4 != 0) {
    std::cerr << "Apply Baudrate Setting failed" << std::endl;
    failed = true;
  }
  if (failed) return -1;
  return 0;
}
int BayCatSPIIO::Open(int) {
  if (IsOpen()) {
    return 0;
  }
  const auto status = apiStatus();
  if (status != 1) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (SPIIsAvailable() != API_OK) {
    std::cerr << "SPI is not available" << std::endl;
    return -1;
  }
  else {
    setIsOpen(true);
  }
  const auto status_update = updateSetting();
  if (status_update != 0) {
    std::cerr << "updateSetting failed: " << status_update << std::endl;
    return status_update;
  }
  return status;
}
int BayCatSPIIO::Close() {
  // do nothing. RAII will take care of closing the API when the last BayCatSPIIO object is destructed
  if (!IsOpen()) {
    return 0;
  }
  setIsOpen(false);
  return 0;
}
int BayCatSPIIO::WriteThenRead(int cs, const uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer, unsigned int rsize, bool csControl) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (csControl) {
    const auto status_cs_low = controlGPIO(cs, false);
    if (status_cs_low != 0) {
      std::cerr << "controlGPIO failed: " << status_cs_low << std::endl;
      return -1;
    }
  }
  uint32_t write_data = 0;
  for (unsigned int i = 0; i < wsize; ++i) {
    write_data = static_cast<uint32_t>(writeBuffer[i]);
    const auto status_write = SPIWriteDataFrame(SPI_SS_SS0_, &write_data); // assuming not using VL_SPI_SS0
    if (status_write != API_OK) {
      std::cerr << "SPIWriteDataFrame failed: " << status_write << std::endl;
      controlGPIO(cs, true);
      return -1;
    }
  }
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: writeBuffer is "; // for debug
  for (unsigned int j = 0; j < wsize; j++) { //for debug
    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(writeBuffer[j]) << " "; //for debug
  } //for debug
  std::cout << std::dec << std::setw(0) << std::endl; //for debug
#endif
  uint32_t read_data = 0;
  for (unsigned int i = 0; i < rsize; ++i) {
    write_data = 1;
    const auto status_write = SPIWriteDataFrame(SPI_SS_SS0_, &write_data);
    const auto status_read = SPIReadDataFrame(&read_data); // assuming not using VL_SPI_SS0
    if (status_read != API_OK) {
      std::cerr << "SPIReadDataFrame failed: " << status_read << std::endl;
      controlGPIO(cs, true);
      return -1;
    }
#ifdef DEBUG_SPI
    std::cout << "read_data " << static_cast<int>(read_data) << std::endl;
#endif
    readBuffer[i] = static_cast<uint8_t>(read_data);
  }
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: readBuffer is "; // for debug
  for (unsigned int j = 0; j < rsize; j++) { //for debug
    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(readBuffer[j]) << " "; //for debug
  } //for debug
  std::cout << std::dec << std::setw(0) << std::endl; //for debug
#endif
  if (csControl) {
    const auto status_cs_high = controlGPIO(cs, true);
    if (status_cs_high != 0) {
      std::cerr << "controlGPIO failed: " << status_cs_high << std::endl;
      return -1;
    }
  }
  return 0;
}
int BayCatSPIIO::WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer, bool csControl) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (csControl) {
    const auto status_cs_low = controlGPIO(cs, false);
    if (status_cs_low != 0) {
      std::cerr << "controlGPIO failed: " << status_cs_low << std::endl;
      return -1;
    }
  }
  uint32_t write_data = 0;
  uint32_t read_data = 0;
  for (unsigned int i = 0; i < size; ++i) {
    write_data = static_cast<uint32_t>(writeBuffer[i]);
    const auto status_write = SPIWriteDataFrame(SPI_SS_SS0_, &write_data); // assuming not using VL_SPI_SS0
    const auto status_read = SPIReadDataFrame(&read_data); // assuming not using VL_SPI_SS0
    if (status_write != API_OK) {
      std::cerr << "SPIWriteDataFrame failed: " << status_write << std::endl;
      controlGPIO(cs, true);
      return -1;
    }
    if (status_read != API_OK) {
      std::cerr << "SPIReadDataFrame failed: " << status_read << std::endl;
      controlGPIO(cs, true);
      return -1;
    }
    readBuffer[i] = static_cast<uint8_t>(read_data);
  }
  if (csControl) {
    const auto status_cs_high = controlGPIO(cs, true);
    if (status_cs_high != 0) {
      std::cerr << "controlGPIO failed: " << status_cs_high << std::endl;
      return -1;
    }
  }
  return 0;
}
int BayCatSPIIO::controlGPIO(const int cs, const bool value) {
  if (cs >= 0 && cs < 16) {
    return controlDIO(cs, value);
  }
  else if (cs <= 24) {
    return controlFPGAGPIO(cs, value);
  }
  else {
    return -1;
  }
}
int BayCatSPIIO::controlDIO(const int cs, const bool value) {
  unsigned char direction = 0;
  const auto status = DIOGetChannelDirection(cs, &direction);
  if (status != API_OK) {
    std::cerr << "DIOGetChannelDirection failed: " << status << std::endl;
    return -1;
  }
  if (direction != DIO_OUTPUT_) {
    std::cerr << "Channel " << cs << " is not set to output" << std::endl;
    DIOSetChannelDirection(cs, DIO_OUTPUT_);
    const auto status = DIOGetChannelDirection(cs, &direction);
    if (status != API_OK) {
      std::cerr << "DIOGetChannelDirection failed: " << status << std::endl;
      return -1;
    }
    std::cout << "Channel " << cs << " output: " << static_cast<int>(direction) << std::endl;
    if (direction != DIO_OUTPUT_) {
      std::cerr << "DIOGetChannelDirection failed: " << "no effects" << std::endl;
      return -1;
    }
  }
  unsigned char value_ = 0;
  if (value) {
    value_ = DIO_CHANNEL_HIGH_;
  }
  else {
    value_ = DIO_CHANNEL_LOW_;
  }
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: CS" << cs << " is set to " << static_cast<int>(value_) << std::endl;
#endif
  DIOSetChannelLevel(cs, value_);
  return 0;
}
int BayCatSPIIO::Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (size <= 0) {
    std::cerr << "Invalid size: size = " << size << std::endl;
    return -1;
  }
  if (csControl){
    const auto status_cs_low = controlGPIO(cs, false);
    if (status_cs_low != 0) {
      std::cerr << "controlGPIO failed: " << status_cs_low << std::endl;
      return -1;
    }
  }
  uint32_t write_data = 0;
  for (unsigned int i = 0; i < size; ++i) {
    write_data = writeBuffer[i];
    const auto status_write = SPIWriteDataFrame(SPI_SS_SS0_, &write_data); // assuming not using VL_SPI_SS0
    if (status_write != API_OK) {
      std::cerr << "SPIWriteDataFrame failed: " << status_write << std::endl;
      controlGPIO(cs, true);
      return -1;
    }
  }
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: writeBuffer is "; // for debug
  for (unsigned int j = 0; j < size; j++) { //for debug
    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(writeBuffer[j]) << " "; //for debug
  } //for debug
  std::cout << std::dec << std::setw(0) << std::endl; //for debug
#endif
  if (csControl) {
    const auto status_cs_high = controlGPIO(cs, true);
    if (status_cs_high != 0) {
      std::cerr << "controlGPIO failed: " << status_cs_high << std::endl;
      return -1;
    }
  }
  return 0;
}
int BayCatSPIIO::controlFPGAGPIO(const int cs, const bool value) {
  bool direction = false;
  unsigned char direction_char = 0;
  const int gpio_id = cs - 16;
  ReadFPGARegister(DIR_GPIO, &direction_char);
  std::cout << "CS " << cs << " direction: " << static_cast<int>(direction_char) << std::endl;
  direction = false;
  WriteFPGARegisterOneChannel(DIR_GPIO, gpio_id, direction);
  ReadFPGARegister(DIR_GPIO, &direction_char);
  std::cout << "CS " << cs << " direction: " << static_cast<int>(direction_char) << std::endl;
  unsigned char out = 32;
  ReadFPGARegister(AUX_OUT, &out);
  std::cout << "AUX_OUT: " << static_cast<int>(out) << std::endl;
  WriteFPGARegisterOneChannel(AUX_OUT, gpio_id, value);
  ReadFPGARegister(AUX_OUT, &out);
  std::cout << "AUX_OUT: " << static_cast<int>(out) << std::endl;
  return 0;
}
int BayCatSPIIO::ReadFPGARegister(unsigned long reg, unsigned char *data) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  if (data == nullptr) {
    std::cerr << "Data pointer is null" << std::endl;
    return -1;
  }
  const auto status = FPGAReadRegister(reg, data);
  if (status != API_OK) {
    std::cerr << "VSL_FPGAReadRegister failed: " << status << std::endl;
    return static_cast<int>(status);
  }
  return 0;
}
int BayCatSPIIO::WriteFPGARegister(unsigned long reg, unsigned char data) {
  if (!IsOpen()) {
    std::cerr << "VersaLogic Library is not initialized" << std::endl;
    return -1;
  }
  const auto status = FPGAWriteRegister(reg, data);
  if (status != API_OK) {
    std::cerr << "VSL_FPGAWriteRegister failed: " << status << std::endl;
    return static_cast<int>(status);
  }
  return 0;
}
int BayCatSPIIO::ReadFPGARegisterOneChannel(const unsigned long reg, const int gpioId, bool *value) {
  if (!value) {
    return -1;
  }
  unsigned char value_raw = 0;
  const int result = ReadFPGARegister(reg, &value_raw);
  if (result == 0)
    *value = static_cast<bool>((value_raw >> gpioId) & 0x1);
  return result;
}
int BayCatSPIIO::WriteFPGARegisterOneChannel(const unsigned long reg, const int gpioId, bool value) {
  unsigned char value_raw = 0;
  const int status_read = ReadFPGARegister(reg, &value_raw);
  if (status_read != 0) {
    return status_read;
  }
  std::cout << static_cast<int>(value_raw) << std::endl;
  if (value) {
    value_raw |= (1 << gpioId);
  }
  else {
    value_raw &= ~(1 << gpioId);
  }
  std::cout << static_cast<int>(value_raw) << std::endl;
  const int status_write = WriteFPGARegister(reg, value_raw);
  return status_write;
}
} // namespace gramsballoon::pgrams