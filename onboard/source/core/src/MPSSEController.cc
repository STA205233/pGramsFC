#include "MPSSEController.hh"
#include "MPSSEUtil.hh"
#include <chrono>
#include <iostream>
#include <thread>
namespace gramsballoon::pgrams::mpsse {

int MPSSEController::initialize() {
  HANDLE_ERROR(FT_SetBitMode(handle_, 0x0, FT_BITMODE_RESET));
  HANDLE_ERROR(FT_SetBitMode(handle_, static_cast<UCHAR>(0xFF), FT_BITMODE_MPSSE));
  HANDLE_ERROR(applySettings());
  HANDLE_ERROR(testConnection());
  HANDLE_ERROR(disableLoopback());

  return 0;
}

int MPSSEController::applySettings() {
  HANDLE_ERROR(FT_SetLatencyTimer(handle_, latencyTimer_));
  HANDLE_ERROR(FT_SetTimeouts(handle_, readTimeout_, writeTimeout_));
  return 0;
}

int MPSSEController::setBaudrate(unsigned int baudrate) {
  baudrate_ = baudrate;
  HANDLE_ERROR(FT_SetBaudRate(handle_, static_cast<ULONG>(baudrate)));

  return 0;
}

int MPSSEController::getBaudrate() {
  return static_cast<int>(baudrate_);
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
  return 0;
}

int MPSSEController::writeSPI(uint8_t *data, unsigned int size, int cs) {
  uint16_t status;
  HANDLE_ERROR(readCurrentPinStatus(status));
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
  HANDLE_ERROR(writeMPSSE(cmdBuffer_));
  cmdBuffer_.clear();
  return 0;
}

int MPSSEController::write_readSPI(uint8_t *dataToSend, unsigned int size, uint8_t *dataToReceive, int cs) {
  uint16_t status;
  HANDLE_ERROR(readCurrentPinStatus(status));
  status &= ~(1 << (cs + 3)); // Keep high byte
  cmdBuffer_.clear();
  cmdBuffer_.push_back(commands::SET_LOW_BYTE_STATE_CMD);
  cmdBuffer_.push_back(status & 0xFF); // Keep current pin states
  cmdBuffer_.push_back(spi_masks::SPI_DIRECTION_MSK);
  cmdBuffer_.push_back(spiWriteReadCommand_);
  cmdBuffer_.push_back(static_cast<uint8_t>((size - 1) & 0xFF)); // Length LSB
  for (unsigned int i = 0; i < size; ++i) {
    cmdBuffer_.push_back(dataToSend[i]);
  }
  cmdBuffer_.push_back(commands::SEND_IMMEDIATE_CMD);
  cmdBuffer_.push_back(commands::SET_LOW_BYTE_STATE_CMD);
  status |= (1 << (cs + 3)); // Keep high byte
  cmdBuffer_.push_back(status & 0xFF); // Keep current pin states
  cmdBuffer_.push_back(spi_masks::SPI_DIRECTION_MSK);
  HANDLE_ERROR(writeMPSSE(cmdBuffer_));
  cmdBuffer_.clear();
  HANDLE_ERROR(readMPSSE(dataToReceive, size));
  return 0;
}

int MPSSEController::writeGPIO(int pin, bool value) {
  uint16_t status;
  HANDLE_ERROR(readCurrentPinStatus(status));
  if (value) {
    status |= (1 << (pin + 3)); // Keep high byte
  }
  else {
    status &= ~(1 << (pin + 3)); // Keep high byte
  }
  cmdBuffer_.clear();
  cmdBuffer_.push_back(commands::SET_LOW_BYTE_STATE_CMD);
  cmdBuffer_.push_back(status & 0xFF); // Keep current pin states
  cmdBuffer_.push_back(spi_masks::SPI_DIRECTION_MSK);
  HANDLE_ERROR(writeMPSSE(cmdBuffer_));
  cmdBuffer_.clear();
  return 0;
}

int MPSSEController::readCurrentPinStatus(uint16_t &status) {
  uint8_t cmd[2] = {commands::GET_LOW_BYTE_STATE_CMD, commands::GET_HIGH_BYTE_STATE_CMD}; // Command to read low byte
  HANDLE_ERROR(writeMPSSE(cmd, sizeof(cmd)));
  uint8_t readData[2] = {0, 0};
  HANDLE_ERROR(readMPSSE(readData, sizeof(readData)));
  status = static_cast<uint16_t>(readData[0]) | (static_cast<uint16_t>(readData[1]) << 8);
  return 0;
}

int MPSSEController::writeMPSSE(std::vector<uint8_t> &data) {
  DWORD numWritten;
  HANDLE_ERROR(FT_Write(handle_, data.data(), static_cast<DWORD>(data.size()), &numWritten));
  if (numWritten != static_cast<DWORD>(data.size())) {
    std::cerr << "MPSSEController::writeMPSSE: Not all bytes were written" << std::endl;
    return -static_cast<int>(FT_OTHER_ERROR);
  }
  return static_cast<int>(numWritten);
}
int MPSSEController::writeMPSSE(uint8_t *data, unsigned int size) {
  DWORD numWritten;
  HANDLE_ERROR(FT_Write(handle_, data, static_cast<DWORD>(size), &numWritten));
  if (numWritten != static_cast<DWORD>(size)) {
    std::cerr << "MPSSEController::writeMPSSE: Not all bytes were written" << std::endl;
    return -static_cast<int>(FT_OTHER_ERROR);
  }
  return static_cast<int>(numWritten);
}
int MPSSEController::readMPSSE(std::vector<uint8_t> &data, unsigned int size) {
  DWORD numRead;
  data.resize(size);
  HANDLE_ERROR(FT_Read(handle_, data.data(), static_cast<DWORD>(size), &numRead));
  if (numRead != static_cast<DWORD>(size)) {
    std::cerr << "MPSSEController::readMPSSE: Not all bytes were read" << std::endl;
    data.resize(numRead);
    return -static_cast<int>(FT_OTHER_ERROR);
  }
  return static_cast<int>(numRead);
}
int MPSSEController::readMPSSE(uint8_t *data, unsigned int size) {
  DWORD numRead;
  HANDLE_ERROR(FT_Read(handle_, data, static_cast<DWORD>(size), &numRead));
  if (numRead != static_cast<DWORD>(size)) {
    std::cerr << "MPSSEController::readMPSSE: Not all bytes were read" << std::endl;
    return -static_cast<int>(FT_OTHER_ERROR);
  }
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
  HANDLE_ERROR(FT_Close(handle_));
  return 0;
}
} // namespace gramsballoon::pgrams::mpsse