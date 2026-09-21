#ifndef GB_I2CInterface_hh
#define GB_I2CInterface_hh 1
#include <cstdint>
namespace gramsballoon::pgrams {
/**
 * @brief Interface class for I2C communication
 * @ingroup device_core
 */
class I2CInterface {
public:
  I2CInterface() = default;
  virtual ~I2CInterface() = default;

protected:
  I2CInterface(const I2CInterface &) = delete;

public:
  /**
   * @brief Open the I2C bus
   * @param[in] ch Channel number to be opened
   */
  virtual int Open(int /*ch*/) { return -1; }
  /**
   * @brief Close the I2C bus
   */
  virtual int Close() { return -1; }
  /**
   * @brief Read a register of an I2C device
   * @param[in] address Address of the target device
   * @param[in] reg_data Register number to be read
   * @param[out] read_data Buffer to store the read data
   * @param[in] read_length Number of the bytes to be read
   */
  virtual int ReadRegister(uint16_t /*address*/, uint8_t /*reg_data*/, uint8_t */*read_data*/, uint32_t /*read_length*/) { return -1; }
  /**
   * @brief Write data to an I2C device
   * @param[in] address Address of the target device
   * @param[in] data Data to be written
   * @param[in] length Number of the bytes to be written
   */
  virtual int Write(uint16_t /*address*/, const uint8_t */*data*/, uint32_t /*length*/) { return -1; }
  /**
   * @brief Read data from an I2C device
   * @param[in] address Address of the target device
   * @param[out] data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   */
  virtual int Read(uint16_t /*address*/, uint8_t */*data*/, uint32_t /*length*/) { return -1; }

  /**
   * @brief Read a register of the device at the address set by setAddress()
   * @param[in] reg_data Register number to be read
   * @param[out] readBuffer Buffer to store the read data
   * @param[in] rsize Number of the bytes to be read
   */
  int ReadRegister(uint8_t reg_data, uint8_t *readBuffer, uint32_t rsize) {
    return ReadRegister(address_, reg_data, readBuffer, rsize);
  }
  /**
   * @brief Write data to the device at the address set by setAddress()
   * @param[in] writeBuffer Data to be written
   * @param[in] size Number of the bytes to be written
   */
  int Write(const uint8_t *writeBuffer, uint32_t size) {
    return Write(address_, writeBuffer, size);
  }
  /**
   * @brief Read data from the device at the address set by setAddress()
   * @param[out] readBuffer Buffer to store the read data
   * @param[in] size Number of the bytes to be read
   */
  int Read(uint8_t *readBuffer, uint32_t size) {
    return Read(address_, readBuffer, size);
  }
  /**
   * @brief Return the address of the device to be accessed by default
   */
  uint16_t Address() const { return address_; }
  /**
   * @brief Set the address of the device to be accessed by default
   * @param[in] address Address of the target device
   */
  void setAddress(uint16_t address) { address_ = address; }

  /**
   * @brief Set the option of the communication
   * @param[in] option Option to be set
   */
  virtual int setOption(int /*option*/) { return -1; }
  /**
   * @brief Apply the current settings to the bus
   */
  virtual int applySetting() { return -1; }

private:
  uint16_t address_ = 0;
  uint32_t option_ = 0;
  bool isOpen_ = false;

public:
  /**
   * @brief Return the option of the communication
   */
  uint32_t Option() const { return option_; }
  /**
   * @brief Return true if the bus is open
   */
  bool IsOpen() const { return isOpen_; }

protected:
  /**
   * @brief Set the flag which shows whether the bus is open
   * @param[in] isOpen Set true when the bus is open
   */
  void setIsOpen(bool isOpen) { isOpen_ = isOpen; }
};
} // namespace gramsballoon::pgrams
#endif //GB_I2CInterface_hh