#ifndef SPIInterface_H
#define SPIInterface_H 1

#include "VCSMapping.hh"
#include <cstdint>
#include <vector>
namespace gramsballoon::pgrams {
/**
 * @brief A class of SPI Interface
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 * @date 2025-05-02 | Shota Arai | Modified to use the FT232H, major refactoring
 * @date 2026-08-11 | Shota Arai | Modified slightly for implementation of the MCP2210
 * @ingroup device_core
 */
class SPIInterface {
public:
  SPIInterface() = default;
  virtual ~SPIInterface() = default;

protected:
  SPIInterface(const SPIInterface &) = delete;
  /**
   * @brief Set the flag which shows whether the bus is open
   * @param[in] isOpen Set true when the bus is open
   */
  void setIsOpen(bool isOpen) { isOpen_ = isOpen; }

public:
  /**
   * @brief Return the baudrate of the SPI transfer
   * @return Baudrate in Hz
   */
  unsigned int Baudrate() const { return baudrate_; }
  /**
   * @brief Apply the current settings to the bus
   * @return 0 on success, negative value on failure
   */
  virtual int updateSetting() { return -1; }
  /**
   * @brief Set the baudrate of the SPI transfer
   * @param[in] baudrate Baudrate in Hz
   */
  virtual void setBaudrate(unsigned int baudrate) {
    baudrate_ = baudrate;
  }
  /**
   * @brief Return whether the bus is open
   * @return True if the bus is open
   */
  bool IsOpen() const { return isOpen_; }
  /**
   * @brief Set the config options, which typically contain the SPI mode and the bit order
   * @param[in] configOptions Config options to be set
   */
  virtual void setConfigOptions(unsigned int configOptions) { configOptions_ = configOptions; }
  /**
   * @brief Return the config options
   * @return Config options
   */
  unsigned int ConfigOptions() const { return configOptions_; }
  /**
   * @brief Open the SPI bus
   * @param[in] ch Channel number to be opened
   * @param[in] path Path of the device file
   * @return 0 on success, negative value on failure
   */
  virtual int Open(int /*ch*/, const char * /*path*/ = "") { return -1; }
  /**
   * @brief Close the SPI bus
   * @return 0 on success, negative value on failure
   */
  virtual int Close() { return -1; }
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
  virtual int WriteThenRead(int /*cs*/, const uint8_t * /*writeBuffer*/, unsigned int /*wsize*/, uint8_t * /*readBuffer*/, unsigned int /*rsize*/, bool /*csControl*/ = true) { return -1; }
  /**
   * @brief Write data to a device and read data from it simultaneously
   * @param[in] cs Chip select channel of the target device
   * @param[in] writeBuffer Data to be written
   * @param[in] size Number of the bytes to be transferred
   * @param[out] readBuffer Buffer to store the read data
   * @param[in] csControl Set true to assert the chip select before the transfer and negate it after the transfer. Set false to leave the chip select untouched, which is used when the caller keeps it asserted over several transfers
   * @return 0 on success, negative value on failure
   */
  virtual int WriteAndRead(int /*cs*/, uint8_t * /*writeBuffer*/, unsigned int /*size*/, uint8_t * /*readBuffer*/, bool /*csControl*/ = true) { return -1; }
  /**
   * @brief Write data to a device
   * @param[in] cs Chip select channel of the target device
   * @param[in] writeBuffer Data to be written
   * @param[in] size Number of the bytes to be written
   * @param[in] csControl Set true to assert the chip select before the transfer and negate it after the transfer. Set false to leave the chip select untouched, which is used when the caller keeps it asserted over several transfers
   * @return 0 on success, negative value on failure
   */
  virtual int Write(int /*cs*/, const uint8_t * /*writeBuffer*/, unsigned int /*size*/, bool /*csControl*/ = true) { return -1; }
  /**
   * @brief Set the level of one GPIO channel
   * @param[in] ch Channel number of the GPIO
   * @param[in] state Specify high / low
   * @return 0 on success, negative value on failure
   */
  virtual int controlGPIO(int /*ch*/, bool /*state*/) { return -1; }
  /**
   *  @brief Control GPIO specified by bit expression
   *  @param[in] csBit Set high to be controlled
   *  @param[in] state Specify high / low
   *  @return 0 on success, negative value on failure
   */
  virtual int controlGPIOBit(uint32_t /*csBit*/, uint32_t /*state*/) { return -1; }
  /**
   * @brief Return the number of the chip select channels available
   * @return Number of the chip select channels
   */
  virtual int MaximumCh() const { return 0; }

  // Convenience functions that use the cs_ member variable
  /**
   * @brief Write data to the device selected by setCs() and then read data from it
   * @param[in] writeBuffer Data to be written
   * @param[in] wsize Number of the bytes to be written
   * @param[out] readBuffer Buffer to store the read data
   * @param[in] rsize Number of the bytes to be read
   * @return 0 on success, negative value on failure
   */
  int WriteThenRead(const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) {
    return WriteThenRead(cs_, writeBuffer, wsize, readBuffer, rsize);
  }
  /**
   * @brief Write data to the device selected by setCs() and read data from it simultaneously
   * @param[in] writeBuffer Data to be written
   * @param[in] wsize Number of the bytes to be transferred
   * @param[out] readBuffer Buffer to store the read data
   * @return 0 on success, negative value on failure
   */
  int WriteAndRead(uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer) {
    return WriteAndRead(cs_, writeBuffer, wsize, readBuffer);
  }
  /**
   * @brief Write data to the device selected by setCs()
   * @param[in] writeBuffer Data to be written
   * @param[in] size Number of the bytes to be written
   * @return 0 on success, negative value on failure
   */
  int Write(const uint8_t *writeBuffer, unsigned int size) {
    return Write(cs_, writeBuffer, size);
  }
  /**
   * @brief Return the chip select channel used by the convenience functions
   * @return Chip select channel
   */
  int getCs() const { return cs_; }
  /**
   * @brief Set the chip select channel used by the convenience functions
   * @param[in] cs Chip select channel of the target device
   */
  void setCs(int cs) {
    cs_ = cs;
  }

  /**
   * @brief Return the list of the available chip select channels
   * @return List of the chip select channels
   */
  virtual const std::vector<int> &Channels() const { return channels_; }

private:
  int cs_ = -1;
  unsigned int baudrate_ = 1000000;
  unsigned int configOptions_ = 0;
  bool isOpen_ = false;
  std::vector<int> channels_;

protected:
  /**
   * @brief Fill the channel list with the channels from 0 to MaximumCh() - 1
   */
  void constructChannels() {
    for (int i = 0; i < MaximumCh(); ++i) {
      channels_.push_back(i);
    }
  }
};

} // namespace gramsballoon::pgrams

#endif /* SPIInterface_H */
