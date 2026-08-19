#include "BayCatSPIIO.hh"
#define DEBUG_SPI 1
#ifdef DEBUG_SPI
#include <iomanip>
#include <iostream>
#endif

#define ERROR_HANDLE(func) \
  {                        \
    const auto ret = func; \
    if (ret < 0) {         \
      return ret;          \
    }                      \
  }

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
  std::cout << "BayCatSPIIO: baudrateList_[baudrate_]: " << baudrateList_[Baudrate()] << std::endl; // For Debug
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
  std::cout << "BayCatSPIIO: options_ & SPI_MODE_MASK: " << (options & SPI_MODE_MASK) << std::endl; // For Debug
#endif
  if (status != API_OK) {
    std::cerr << "SPISetMode failed: " << status << std::endl;
    failed = true;
  }

  const unsigned int shift_direction = (options & SPI_SHIFT_DIRECTION_MASK) >> SPI_SHIFT_DIRECTION_OFFSET;
  if (shift_direction != static_cast<unsigned int>(SPI_DIR_LEFT_) && shift_direction != static_cast<unsigned int>(SPI_DIR_RIGHT_)) {
    std::cerr << "ShiftDirection is invalid: " << SPI_DIR_RIGHT_ << " or " << SPI_DIR_LEFT_ << " are allowed.";
    failed = true;
  }
  else {
    const int status2 = SPISetShiftDirection(shift_direction);
#ifdef DEBUG_SPI
    std::cout << "BayCatSPIIO: shift_direction: " << shift_direction << std::endl; // For Debug
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
int BayCatSPIIO::Open(int, const char *) {
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
  return 0;
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
  for (unsigned int j = 0; j < wsize; j++) { // for debug
    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(writeBuffer[j]) << " "; // for debug
  } // for debug
  std::cout << std::dec << std::setw(0) << std::endl; // for debug
#endif
  uint32_t read_data = 0;
  for (unsigned int i = 0; i < rsize; ++i) {
    write_data = 1;
    const auto status_write = SPIWriteDataFrame(SPI_SS_SS0_, &write_data);
    if (status_write != API_OK) {
      std::cerr << "SPIWriteDataFrame failed: " << status_write << std::endl;
      controlGPIO(cs, true);
      return -1;
    }
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
  for (unsigned int j = 0; j < rsize; j++) { // for debug
    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(readBuffer[j]) << " "; // for debug
  } // for debug
  std::cout << std::dec << std::setw(0) << std::endl; // for debug
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
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: writeBuffer is "; // for debug
  for (unsigned int j = 0; j < size; j++) { // for debug
    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(writeBuffer[j]) << " "; // for debug
  } // for debug
  std::cout << std::dec << std::setw(0) << std::endl; // for debug
#endif
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
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: readBuffer is "; // for debug
  for (unsigned int j = 0; j < size; j++) { // for debug
    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(readBuffer[j]) << " "; // for debug
  } // for debug
  std::cout << std::dec << std::setw(0) << std::endl; // for debug
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
int BayCatSPIIO::controlGPIO(const int cs, const bool value) {
  if (cs >= 0 && cs < 16) {
    return controlDIO(cs, value);
  }
  else if (cs <= 24) {
    const uint32_t bit = (1u << cs);
    return controlFPGAGPIO(bit, value ? bit : 0u);
  }
  else {
    return -1;
  }
}

int BayCatSPIIO::controlGPIOBit(const uint32_t csBit, const uint32_t value) {
  constexpr uint32_t LOWER_MSK = 0xffff;
  bool succeeded = true;
  if ((csBit & LOWER_MSK) != 0) {
    for (int ibit = 0; ibit < 16; ++ibit) {
      if (((csBit >> ibit) & 0x1) == 0x1) {
        const bool bitValue = ((value >> ibit) & 0x1) == 0x1;
        const int ret = controlDIO(ibit, bitValue);
        succeeded = (ret >= 0);
        if (!succeeded) { break; }
      }
    }
  }

  if (!succeeded) { return -1; }
  constexpr uint32_t UPPER_MSK = 0xffff0000;
  const uint32_t csBit_upper = csBit & UPPER_MSK;
  if (csBit_upper != 0) {
    const int ret = controlFPGAGPIO(csBit_upper, value & UPPER_MSK);
    if (ret < 0) { return -1; }
  }
  return 0;
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

  // final check
  const unsigned char read_back = DIOGetChannelLevel(cs);
#ifdef DEBUG_SPI
  std::cout << "BayCatSPIIO: CS" << cs << " read back " << static_cast<int>(read_back) << std::endl;
#endif
  if (value != (read_back == DIO_CHANNEL_HIGH_)) {
    unsigned char direction_after = DIO_UNKNOWN_;
    DIOGetChannelDirection(cs, &direction_after);
    std::cerr << "ControlGPIOBit: failed final check. CS: " << cs
              << ", expected: " << static_cast<int>(value_)
              << ", read back: " << static_cast<int>(read_back)
              << ", direction: " << static_cast<int>(direction_after)
              << " (" << DIO_OUTPUT_ << " = output)" << std::endl;
    return -1;
  }
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
  if (csControl) {
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
  for (unsigned int j = 0; j < size; j++) { // for debug
    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(writeBuffer[j]) << " "; // for debug
  } // for debug
  std::cout << std::dec << std::setw(0) << std::endl; // for debug
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
int BayCatSPIIO::controlFPGAGPIO(uint32_t csBit, uint32_t value) {
  csBit = (csBit >> 16);
  value = (value >> 16);
#ifdef DEBUG_SPI
  std::cout << "controlFPGAGPIO called with " << std::hex << csBit << ", " << value << std::dec << std::endl;
#endif
  const bool direction = true; // Output
  ERROR_HANDLE(WriteFPGARegisterMultiChannel(DIR_GPIO, csBit, direction))
  ERROR_HANDLE(WriteFPGARegisterMultiChannel(AUX_OUT, csBit, value))

  // final check
  unsigned char aux_in = 0;
  ERROR_HANDLE(ReadFPGARegister(AUX_IN, &aux_in))
#ifdef DEBUG_SPI
  std::cout << "AUX_IN (final check): " << std::hex << static_cast<int>(aux_in) << std::dec << std::endl;
#endif
  const uint32_t ref = value & csBit;
  const uint32_t actual = static_cast<uint32_t>(aux_in) & csBit;
  if (actual != ref) {
    unsigned char dir_raw = 0;
    unsigned char out_raw = 0;
    ReadFPGARegister(DIR_GPIO, &dir_raw);
    ReadFPGARegister(AUX_OUT, &out_raw);
    std::cerr << "ControlGPIOBit: failed final check. " << std::hex
              << "csBit(>>16): " << csBit
              << ", expected: " << ref
              << ", read back: " << actual
              << ", mismatched bits: " << (actual ^ ref)
              << ", AUX_IN(raw): " << static_cast<int>(aux_in)
              << ", AUX_OUT(raw): " << static_cast<int>(out_raw)
              << ", DIR_GPIO(raw): " << static_cast<int>(dir_raw) << " (1 = output)"
              << std::dec << std::endl;
    return -1;
  }
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

int BayCatSPIIO::WriteFPGARegisterMultiChannel(const unsigned long reg, const uint32_t bitExpression, const bool value) {
  return WriteFPGARegisterMultiChannel(reg, bitExpression, value ? bitExpression : 0u);
}

int BayCatSPIIO::WriteFPGARegisterMultiChannel(const unsigned long reg, const uint32_t bitExpression, const uint32_t data) {
#ifdef DEBUG_SPI
  std::cout << "WriteFPGARegisterMultiChannel called with " << reg << ", " << std::hex << bitExpression << ", " << data << std::dec << std::endl;
#endif
  unsigned char value_raw = 0;
  const int status_read = ReadFPGARegister(reg, &value_raw);
  if (status_read != 0) {
    return status_read;
  }
#ifdef DEBUG_SPI
  std::cout << "WriteFPGARegisterMultiChannel value before writing: " << std::hex << static_cast<int>(value_raw) << std::dec << std::endl;
#endif
  value_raw = static_cast<unsigned char>((value_raw & ~bitExpression) | (data & bitExpression));
#ifdef DEBUG_SPI
  std::cout << "WriteFPGARegisterMultiChannel value after writing: " << std::hex << static_cast<int>(value_raw) << std::dec << std::endl;
#endif
  const int status_write = WriteFPGARegister(reg, value_raw);
  return status_write;
}
} // namespace gramsballoon::pgrams