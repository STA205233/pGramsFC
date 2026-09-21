#ifndef GRAMSBalloon_FT232HIO_hh
#define GRAMSBalloon_FT232HIO_hh 1
#include "MPSSEController.hh"
#include "MPSSEDeviceManager.hh"
#include "SPIInterface.hh"
#include <cstdint>
#include <memory>
namespace gramsballoon::pgrams {
/**
 * @brief Control Class for FT232H
 * @author Shota Arai
 * @date 2025-**-** | Shota Arai | First version
 * @date 2026-07-10 | Shota Arai | Added controlGPIOBit
 * @ingroup device_core
 */
class FT232HIO: public SPIInterface {
private:
public:
  FT232HIO();
  virtual ~FT232HIO() = default;
  /**
   * @brief Return the number of the chip select channels available
   * @return Number of the chip select channels
   */
  int MaximumCh() const override { return 8; }

protected:
  FT232HIO(const FT232HIO &) = delete;

public:
  struct config {
    static constexpr unsigned int SPI_MODE0 = 0x0;
    static constexpr unsigned int SPI_MODE1 = 0x1;
    static constexpr unsigned int SPI_MODE2 = 0x2;
    static constexpr unsigned int SPI_MODE3 = 0x3;
  };
  /**
   * @brief Open the FT232H device and apply the current settings
   * @param[in] channel Channel number of the device to be opened
   * @return 0 on success, negative value on failure
   */
  int Open(int channel, const char *) override;
  /**
   * @brief Close the FT232H device
   * @return 0 on success, negative value on failure
   */
  int Close() override {
    if (!IsOpen()) {
      return 0;
    }
    if (mpsseController_) {
      return mpsseController_->close();
    }
    setIsOpen(false);
    return 0;
  }
  /**
   * @brief Write data to a device and then read data from it
   * @param[in] cs Chip select channel of the target device
   * @param[in] writeBuffer Data to be written
   * @param[in] wsize Number of the bytes to be written
   * @param[out] readBuffer Buffer to store the read data
   * @param[in] rsize Number of the bytes to be read
   * @param[in] csControl Set true to assert the chip select before the transfer and negate it after the transfer. Set false to leave the chip select untouched, which is used when the caller keeps it asserted over several transfers
   * @return 0 on success, negative value on failure
   */
  int WriteThenRead(int cs, const uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer, unsigned int rsize, bool csControl) override;
  /**
   * @brief Write data to a device and read data from it simultaneously
   * @param[in] cs Chip select channel of the target device
   * @param[in] writeBuffer Data to be written
   * @param[in] size Number of the bytes to be transferred
   * @param[out] readBuffer Buffer to store the read data
   * @param[in] csControl Set true to assert the chip select before the transfer and negate it after the transfer. Set false to leave the chip select untouched, which is used when the caller keeps it asserted over several transfers
   * @return 0 on success, negative value on failure
   */
  int WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer, bool csControl) override;
  /**
   * @brief Write data to a device
   * @param[in] cs Chip select channel of the target device
   * @param[in] writeBuffer Data to be written
   * @param[in] size Number of the bytes to be written
   * @param[in] csControl Set true to assert the chip select before the transfer and negate it after the transfer. Set false to leave the chip select untouched, which is used when the caller keeps it asserted over several transfers
   * @return 0 on success, negative value on failure
   */
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) override;
  /**
   * @brief Set the level of one GPIO channel
   * @param[in] cs Channel number of the GPIO
   * @param[in] value Level to be set
   * @return 0 on success, negative value on failure
   */
  int controlGPIO(int cs, bool value) override;
  /**
   * @brief Set the levels of several GPIO channels at once, where each bit corresponds to one channel
   * @param[in] cs Channels to be controlled. The n-th bit set to 1 means that the channel n is updated
   * @param[in] value Levels to be set, where the n-th bit gives the level of the channel n. The bits not selected by cs are ignored
   * @return 0 on success, negative value on failure
   */
  int controlGPIOBit(uint32_t cs, uint32_t value) override;
  /**
   * @brief Apply the current settings, i.e. the SPI mode and the baudrate, to the device
   * @return 0 on success, negative value on failure
   */
  int updateSetting() override;

private:
  std::shared_ptr<mpsse::MPSSEDeviceManager> mpsseDeviceManager_;
  std::shared_ptr<mpsse::MPSSEController> mpsseController_;
  std::vector<uint8_t> writeBuffer_;
  std::vector<uint8_t> readBuffer_;
};

} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_FT232HIO_hh