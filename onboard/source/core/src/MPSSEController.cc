#include "MPSSEController.hh"
#include "MPSSEUtil.hh"
#include <chrono>
#include <iostream>
#include <thread>
namespace gramsballoon::pgrams::mpsse {
MPSSEController::~MPSSEController() {
  close();
}

int MPSSEController::initialize() {
  HANDLE_ERROR(FT_SetBitMode(handle_, 0x0, FT_BITMODE_RESET));
  HANDLE_ERROR(FT_SetBitMode(handle_, static_cast<UCHAR>(0xFF), FT_BITMODE_MPSSE));
  const auto s1 = applySettings();
  if (s1 != 0) {
    return s1;
  }
  const auto s2 = testConnection();
  if (s2 != 0) {
    return s2;
  }
  const auto s3 = disableLoopback();
  if (s3 != 0) {
    return s3;
  }
  return 0;
}

unsigned int MPSSEController::calDivider(unsigned int baudrate) {
  return (60000000 / (2 * baudrate)) - 1;
}

int MPSSEController::applySettings() {
  {
    const unsigned int divider_setting = calDivider(getBaudrate());
    uint8_t cmd[] = {commands::DISABLE_5_DIVISION, commands::SET_DIVIDER, static_cast<uint8_t>(divider_setting & 0xFF), static_cast<uint8_t>((divider_setting >> 8) & 0xFF)};
    const auto status = writeMPSSE(&cmd[0], sizeof(cmd) / sizeof(uint8_t));
    if (status < 0) {
      std::cerr << "Changing Baudrate is failed: " << status << std::endl;
    }
  }
  DBG("baudrate is set to " << getBaudrate());
  HANDLE_ERROR(FT_SetLatencyTimer(handle_, latencyTimer_));
  DBG("latencyTimer is set to " << latencyTimer_);
  HANDLE_ERROR(FT_SetTimeouts(handle_, readTimeout_, writeTimeout_));
  DBG("readTimeout is set to " << readTimeout_);
  DBG("writeTimeout is set to " << writeTimeout_);
  setSPIMode(spiMode_);
  return 0;
}

void MPSSEController::setBaudrate(unsigned int baudrate) {
  baudrate_ = baudrate;
}

unsigned int MPSSEController::getBaudrate() {
  return baudrate_;
}
int MPSSEController::setSPIMode(int mode, bool byteMode, bool msbFirst) {
  if (mode < 0 || mode > 3) {
    std::cerr << "MPSSEController::setSPIMode: Invalid SPI mode: " << mode << std::endl;
    return -1;
  }
  const uint8_t byte_msk = byteMode ? spi_masks::SPI_BYTE_MODE : spi_masks::SPI_BIT_MODE;
  const uint8_t msb_msk = msbFirst ? spi_masks::SPI_MSB_FIRST : spi_masks::SPI_LSB_FIRST;
  spiWriteCommand_ = spi_masks::SPI_MODE_MSK(mode) | spi_masks::SPI_WRITE_MSK | byte_msk | msb_msk;
  spiReadCommand_ = spi_masks::SPI_MODE_MSK(mode) | spi_masks::SPI_READ_MSK | byte_msk | msb_msk;
  spiWriteReadCommand_ = spi_masks::SPI_MODE_MSK(mode) | spi_masks::SPI_WRITE_MSK | spi_masks::SPI_READ_MSK | byte_msk | msb_msk;
  spiMode_ = mode;
  uint8_t cmd[3];
  cmd[0] = commands::SET_LOW_BYTE_STATE_CMD;
  cmd[1] = spi_masks::SPI_Initial_State(mode); // Initial state of pins
  cmd[2] = spi_masks::SPI_DIRECTION_MSK; // Direction of pins:
  writeMPSSE(cmd, sizeof(cmd));
  return 0;
}

int MPSSEController::writeSPI(uint8_t *data, unsigned int size, int cs) {
  uint16_t status;
  readCurrentPinStatus(status);
  status &= ~(1 << (cs + 3)); // Keep high byte
  cmdBuffer_.clear();
  cmdBuffer_.push_back(commands::SET_LOW_BYTE_STATE_CMD);
  cmdBuffer_.push_back(status & 0xFF); // Keep current pin states
  cmdBuffer_.push_back(spi_masks::SPI_DIRECTION_MSK);
  cmdBuffer_.push_back(spiWriteCommand_);
  cmdBuffer_.push_back(static_cast<uint8_t>((size - 1) & 0xFF)); // Length LSB
  for (unsigned int i = 0; i < size; ++i) {
    cmdBuffer_.push_back(data[i]);
  }
  cmdBuffer_.push_back(commands::SET_LOW_BYTE_STATE_CMD);
  status |= (1 << (cs + 3)); // Keep high byte
  cmdBuffer_.push_back(status & 0xFF); // Keep current pin states
  cmdBuffer_.push_back(spi_masks::SPI_DIRECTION_MSK);
  writeMPSSE(cmdBuffer_);
  cmdBuffer_.clear();
  return 0;
}

int MPSSEController::write_readSPI(uint8_t *dataToSend, unsigned int size, uint8_t *dataToReceive, int cs) {
  uint16_t status;
  readCurrentPinStatus(status);
  status &= ~(1 << (cs + 3)); // Keep high byte
  cmdBuffer_.clear();
  cmdBuffer_.push_back(commands::SET_LOW_BYTE_STATE_CMD);
  cmdBuffer_.push_back(status & 0xFF); // Keep current pin states
  cmdBuffer_.push_back(spi_masks::SPI_DIRECTION_MSK);
  cmdBuffer_.push_back(spiWriteReadCommand_);
  cmdBuffer_.push_back(static_cast<uint8_t>((size - 1) & 0xFF)); // Length LSB
  cmdBuffer_.push_back(static_cast<uint8_t>(((size - 1) >> 8) & 0xFF)); // Length MSB
  for (unsigned int i = 0; i < size; ++i) {
    cmdBuffer_.push_back(dataToSend[i]);
  }
  cmdBuffer_.push_back(commands::SET_LOW_BYTE_STATE_CMD);
  status |= (1 << (cs + 3)); // Keep high byte
  cmdBuffer_.push_back(status & 0xFF); // Keep current pin states
  cmdBuffer_.push_back(spi_masks::SPI_DIRECTION_MSK);
  cmdBuffer_.push_back(commands::SEND_IMMEDIATE_CMD);
  writeMPSSE(cmdBuffer_);
  cmdBuffer_.clear();
  return readMPSSE(dataToReceive, size);
}

int MPSSEController::writeGPIO(int pin, bool value) {
  uint16_t status;
  readCurrentPinStatus(status);
  if (value) {
    status |= (1 << (pin + 3)); // Keep high byte
  }
  else {
    status &= ~(1 << (pin + 3)); // Keep high byte
  }
  return writeGPIORaw(status);
}
int MPSSEController::writeGPIORaw(const uint16_t status) {
  cmdBuffer_.clear();
  cmdBuffer_.push_back(commands::SET_LOW_BYTE_STATE_CMD);
  cmdBuffer_.push_back(status & 0xFF); // Keep current pin states
  cmdBuffer_.push_back(spi_masks::SPI_DIRECTION_MSK);
  cmdBuffer_.push_back(commands::SET_HIGH_BYTE_STATE_CMD);
  cmdBuffer_.push_back((status >> 8) & 0xFF);
  cmdBuffer_.push_back(0xFF); // All high byte pins as output
  const int write_status = writeMPSSE(cmdBuffer_);
  cmdBuffer_.clear();
  return write_status;
}
int MPSSEController::writeGPIOMul(const uint32_t state) {
  uint16_t status;
  readCurrentPinStatus(status);
  status = static_cast<uint16_t>(((state << 3) & 0xFFF8) + (status & 0x07));
  return writeGPIORaw(status);
}

int MPSSEController::readCurrentPinStatus(uint16_t &status) {
  uint8_t cmd[2] = {commands::GET_LOW_BYTE_STATE_CMD, commands::GET_HIGH_BYTE_STATE_CMD}; // Command to read low byte
  const int write_status = writeMPSSE(cmd, sizeof(cmd));
  uint8_t readData[2] = {0, 0};
  const int read_status = readMPSSE(readData, sizeof(readData));
  status = static_cast<uint16_t>(readData[0]) | (static_cast<uint16_t>(readData[1]) << 8);
  return write_status < 0 ? write_status : read_status;
}

int MPSSEController::writeMPSSE(std::vector<uint8_t> &data) {
  return writeMPSSE(data.data(), data.size());
}
int MPSSEController::writeMPSSE(uint8_t *data, unsigned int size) {
#ifdef SPI_DEBUG
  std::cout << __func__ << " is called with arguments ";
  for (unsigned int i = 0; i < size; i++) {
    std::cout << std::hex << static_cast<int>(data[i]) << " " << std::dec;
  }
  std::cout << std::endl;
#endif
  DWORD numWritten;
  HANDLE_ERROR(FT_Write(handle_, data, static_cast<DWORD>(size), &numWritten));
  if (numWritten != static_cast<DWORD>(size)) {
    std::cerr << "MPSSEController::writeMPSSE: Not all bytes were written" << std::endl;
    return -static_cast<int>(FT_OTHER_ERROR);
  }
  return static_cast<int>(numWritten);
}
int MPSSEController::readMPSSE(std::vector<uint8_t> &data, unsigned int size) {
  return readMPSSE(data.data(), size);
}
int MPSSEController::readMPSSE(uint8_t *data, unsigned int size) {
  DWORD numRead = 0;
  for (int i = 0; i < 100; i++) {
    DWORD numReadAtOnce = 0;
    HANDLE_ERROR(FT_Read(handle_, data, static_cast<DWORD>(size), &numReadAtOnce));
    numRead += numReadAtOnce;
    if (numRead == size) {
      break;
    }
    std::cout << numRead << std::endl;
  }
  if (numRead != static_cast<DWORD>(size)) {
    std::cerr << "MPSSEController::readMPSSE: Not all bytes were read" << std::endl;
    return -static_cast<int>(FT_OTHER_ERROR);
  }
#ifdef SPI_DEBUG
  std::cout << __func__ << " is called with arguments and result: ";
  for (unsigned int i = 0; i < size; i++) {
    std::cout << std::hex << static_cast<int>(data[i]) << " " << std::dec;
  }
  std::cout << std::endl;
#endif
  return static_cast<int>(numRead);
}
int MPSSEController::testConnection() {
  // Test 1
  uint8_t testData[1] = {commands::MPSSE_TEST_CMD1};
  const int write_status = writeMPSSE(testData, sizeof(testData));
  if (write_status < 0) {
    std::cerr << "MPSSEController::testConnection: Write failed" << std::endl;
    return write_status;
  }
  uint8_t readData[2] = {0, 0};
  const int read_status = readMPSSE(readData, sizeof(readData));
  if (read_status < 0) {
    std::cerr << "MPSSEController::testConnection: Read failed" << std::endl;
    return read_status;
  }
  if (readData[0] != commands::MPSSE_ERROR_CMD_INVALID && readData[1] != commands::MPSSE_TEST_CMD1) {
    std::cerr << "MPSSEController::testConnection: Data mismatch. Sent 0xAA, received 0x" << std::hex << static_cast<int>(readData[0]) << std::dec << std::endl;
    return -1;
  }

  // Test 2
  testData[0] = commands::MPSSE_TEST_CMD2;
  const int write_status2 = writeMPSSE(testData, sizeof(testData));
  if (write_status2 < 0) {
    std::cerr << "MPSSEController::testConnection: Write failed" << std::endl;
    return write_status2;
  }
  readData[0] = 0;
  readData[1] = 0;
  const int read_status2 = readMPSSE(readData, sizeof(readData));
  if (read_status2 < 0) {
    std::cerr << "MPSSEController::testConnection: Read failed" << std::endl;
    return read_status2;
  }
  if (readData[0] != commands::MPSSE_ERROR_CMD_INVALID && readData[1] != commands::MPSSE_TEST_CMD2) {
    std::cerr << "MPSSEController::testConnection: Data mismatch. Sent 0xAB, received 0x" << std::hex << static_cast<int>(readData[0]) << std::dec << std::endl;
    return -1;
  }

  return 0;
}

int MPSSEController::disableLoopback() {
  uint8_t cmd[1] = {commands::LOOPBACK_DISABLE_CMD};
  const int write_status = writeMPSSE(cmd, sizeof(cmd));
  if (write_status < 0) {
    std::cerr << "MPSSEController::disableLoopback: Write failed" << std::endl;
    return write_status;
  }
  return 0;
}

int MPSSEController::close() {
  if (opened_) {
    opened_ = false;
    HANDLE_ERROR(FT_Close(handle_));
  }
  return 0;
}
} // namespace gramsballoon::pgrams::mpsse