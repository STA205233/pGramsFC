#ifndef GRAMSBalloon_MPSSEUtil_hh
#define GRAMSBalloon_MPSSEUtil_hh 1
#include <iostream>
#ifdef HANDLE_ERROR
#error "HANDLE_ERROR macro is already defined. Check including files and whether include is put on src file or not. This file is not intended to be included in header files."
#else
#define HANDLE_ERROR(status)                                             \
  if (status != FT_OK) {                                                 \
    std::cerr << "Error in " << __func__ << ": " << status << std::endl; \
    return -static_cast<int>(status);                                    \
  }
#endif
namespace gramsballoon::pgrams::mpsse {
namespace commands {
constexpr uint8_t MPSSE_ERROR_CMD_INVALID = 0xFA;
constexpr uint8_t MPSSE_TEST_CMD1 = 0xAA;
constexpr uint8_t MPSSE_TEST_CMD2 = 0xAB;

constexpr uint8_t SET_LOW_BYTE_STATE_CMD = 0x80;
constexpr uint8_t GET_LOW_BYTE_STATE_CMD = 0x81;
constexpr uint8_t SET_HIGH_BYTE_STATE_CMD = 0x82;
constexpr uint8_t GET_HIGH_BYTE_STATE_CMD = 0x83;

constexpr uint8_t LOOPBACK_DISABLE_CMD = 0x85;
constexpr uint8_t SEND_IMMEDIATE_CMD = 0x87;
} // namespace commands
namespace spi_masks {
constexpr uint8_t SPI_WRITE_CLK_FALLING = 0x01;
constexpr uint8_t SPI_WRITE_CLK_RISING = 0x00;
constexpr uint8_t SPI_BYTE_MODE = 0x00;
constexpr uint8_t SPI_BIT_MODE = 0x02;
constexpr uint8_t SPI_LSB_FIRST = 0x08;
constexpr uint8_t SPI_MSB_FIRST = 0x00;
constexpr uint8_t SPI_WRITE_MSK = 0x10;
constexpr uint8_t SPI_READ_MSK = 0x20;
constexpr uint8_t SPI_MODE_MSK(int mode) {
  switch (mode) {
  case 0:
    return SPI_WRITE_CLK_FALLING;
  case 1:
    return SPI_WRITE_CLK_RISING;
  case 2:
    return SPI_WRITE_CLK_RISING;
  case 3:
    return SPI_WRITE_CLK_FALLING;
  default:
    break;
  }
  throw std::invalid_argument("Invalid SPI mode");
}
constexpr uint8_t SPI_Initial_State(int mode) {
  switch (mode) {
  case 0:
    return 0b1000;
  case 1:
    return 0b1000;
  case 2:
    return 0b1001;
  case 3:
    return 0b1001;
  default:
    break;
  }
  throw std::invalid_argument("Invalid SPI mode");
}

constexpr uint8_t SPI_DIRECTION_MSK = 0x0B; // SK, DO, CS as output
constexpr uint8_t SPI_CLK_DO_DI = 0b00000111; // SK, DO, as output

} // namespace spi_masks
} // namespace gramsballoon::pgrams::mpsse
#endif // GRAMSBalloon_MPSSEUtil_hh