#include <sys/types.h>
#ifndef GRAMSBalloon_BayCatControl_hh
#define GRAMSBalloon_BayCatControl_hh 1
#include "BayCatAPICaller.hh"
#include "SPIInterface.hh"
#include <cstdint>
#include <map>
namespace gramsballoon::pgrams {
/**
 * @brief A class to control the SPI interface of BayCat.
 * @author Shota Arai
 * @date 2025-**-** | Shota Arai | Created
 * @date 2026-04-10 | Shota Arai | Refactored to use BayCatAPICaller
 * @date 2026-07-10 | Shota Arai | Added controlGPIOBit
 * @ingroup device_core
 */
class BayCatSPIIO: public SPIInterface, public BayCatAPICaller {
public:
  static constexpr unsigned int SPI_MODE_MASK = 0x3;
  static constexpr unsigned int SPI_SHIFT_DIRECTION_MASK = 0x4;
  static constexpr unsigned int SPI_SHIFT_DIRECTION_OFFSET = 2;
  /**
   * @brief Compose the config options from the SPI mode and the bit order
   * @param[in] mode SPI mode (0-3)
   * @param[in] shiftDirection Bit order of the transfer, SPI_DIR_LEFT_ for MSB first and SPI_DIR_RIGHT_ for LSB first
   */
  static unsigned int MakeOption(unsigned int mode, unsigned int shiftDirection) {
    return (mode & SPI_MODE_MASK) | (shiftDirection << SPI_SHIFT_DIRECTION_OFFSET);
  }

  static constexpr unsigned long DIR_GPIO = 0x21;
  static constexpr unsigned long AUX_OUT = 0x23;
  static constexpr unsigned long AUX_IN = 0x24;

public:
  BayCatSPIIO();
  virtual ~BayCatSPIIO() = default;
  BayCatSPIIO(const BayCatSPIIO &) = delete;
  /**
   * @brief Return the number of the chip select channels available
   */
  int MaximumCh() const override { return 24; }

private:
  std::map<int, unsigned int> baudrateList_;

public:
  /**
   * @brief Apply the current settings, i.e. the mode, the bit order, the frame size and the baudrate
   */
  int updateSetting() override;
  /**
   * @brief Set the SPI baudrate. Only the frequencies supported by the API are accepted
   * @param[in] baudrate Baudrate in Hz
   */
  void setBaudrate(unsigned int baudrate) override;
  /**
   * @brief Open the SPI bus and apply the current settings
   * @param[in] channel Channel number of the bus, not used
   * @param[in] path Path of the device file, not used
   */
  int Open(int channel, const char *path) override;
  /**
   * @brief Close the SPI bus
   */
  int Close() override;
  /**
   * @brief Write data to a device and then read data from it
   * @param[in] cs Chip select channel of the target device
   * @param[in] writeBuffer Data to be written
   * @param[in] wsize Number of the bytes to be written
   * @param[out] readBuffer Buffer to store the read data
   * @param[in] rsize Number of the bytes to be read
   * @param[in] csControl Set true to assert the chip select before the transfer and negate it after the transfer. Set false to leave the chip select untouched, which is used when the caller keeps it asserted over several transfers
   */
  int WriteThenRead(int cs, const uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer, unsigned int rsize, bool csControl) override;
  /**
   * @brief Write data to a device and read data from it simultaneously
   * @param[in] csControl Set true to assert the chip select before the transfer and negate it after the transfer. Set false to leave the chip select untouched, which is used when the caller keeps it asserted over several transfers
   */
  int WriteAndRead(int /*cs*/, uint8_t * /*writeBuffer*/, unsigned int /*size*/, uint8_t * /*readBuffer*/, bool csControl) override;
  /**
   * @brief Write data to a device
   * @param[in] cs Chip select channel of the target device
   * @param[in] writeBuffer Data to be written
   * @param[in] size Number of the bytes to be written
   * @param[in] csControl Set true to assert the chip select before the transfer and negate it after the transfer. Set false to leave the chip select untouched, which is used when the caller keeps it asserted over several transfers
   */
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) override;
  /**
   * @brief Set the level of one GPIO channel, which is a DIO channel for cs < 16 and an FPGA GPIO channel otherwise
   * @param[in] cs Channel number of the GPIO
   * @param[in] value Level to be set
   */
  int controlGPIO(int cs, bool value) override;
  /**
   * @brief Set the levels of several GPIO channels at once, where each bit corresponds to one channel
   * @param[in] csBit Channels to be controlled. The n-th bit set to 1 means that the channel n is updated
   * @param[in] value Levels to be set, where the n-th bit gives the level of the channel n. The bits not selected by csBit are ignored
   */
  int controlGPIOBit(uint32_t csBit, uint32_t value) override;
  /**
   * @brief Write one FPGA register
   * @param[in] reg Register address
   * @param[in] data Value to be written
   */
  int WriteFPGARegister(unsigned long reg, unsigned char data);
  /**
   * @brief Write the same level to several bits of an FPGA register, where each bit corresponds to one channel
   * @param[in] reg Register address
   * @param[in] bitexpression Bits to be written. The n-th bit set to 1 means that the bit n is updated
   * @param[in] data Level to be written to all the selected bits
   */
  int WriteFPGARegisterMultiChannel(unsigned long reg, uint32_t bitexpression, bool data);
  /**
   * @brief Write several bits of an FPGA register at once, where each bit corresponds to one channel
   * @param[in] reg Register address
   * @param[in] bitexpression Bits to be written. The n-th bit set to 1 means that the bit n is updated
   * @param[in] data Levels to be written, where the n-th bit gives the level of the bit n. The bits not selected by bitexpression are ignored
   */
  int WriteFPGARegisterMultiChannel(unsigned long reg, uint32_t bitexpression, uint32_t data);
  /**
   * @brief Read one FPGA register
   * @param[in] reg Register address
   * @param[out] data Buffer to store the read value
   */
  int ReadFPGARegister(unsigned long reg, unsigned char *data);

private:
  /**
   * @brief Apply the baudrate setting to the API
   */
  int applyBaudrateSetting();
  /**
   * @brief Set the levels of several FPGA GPIO channels at once, where each bit corresponds to one channel
   * @param[in] bitExpression Channels to be controlled. The n-th bit set to 1 means that the channel n is updated
   * @param[in] value Levels to be set, where the n-th bit gives the level of the channel n. The bits not selected by bitExpression are ignored
   */
  int controlFPGAGPIO(uint32_t bitExpression, uint32_t value);
  /**
   * @brief Set the level of one DIO channel
   * @param[in] cs Channel number of the DIO
   * @param[in] value Level to be set
   */
  int controlDIO(int cs, bool value);
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_BayCatSPIInterface_hh