#ifndef GB_I2CInterface_hh
#define GB_I2CInterface_hh 1
#include <cstdint>
namespace gramsballoon::pgrams {
/**
 * @brief Interface class for I2C communication
 */
class I2CInterface {
public:
  I2CInterface() = default;
  virtual ~I2CInterface() = default;

protected:
  I2CInterface(const I2CInterface &) = delete;

public:
  virtual int Open(int) { return -1; }
  virtual int Close() { return -1; }
  virtual int WriteThenRead(uint16_t address, const uint8_t *reg_data, uint32_t length, uint8_t *read_data, uint32_t read_length) { return -1; }
  virtual int Write(uint16_t address, const uint8_t *data, uint32_t length) { return -1; }
  virtual int Read(uint16_t address, uint8_t *data, uint32_t length) { return -1; }

  int WriteThenRead(const uint8_t *writeBuffer, uint32_t wsize, uint8_t *readBuffer, uint32_t rsize) {
    return WriteThenRead(address_, writeBuffer, wsize, readBuffer, rsize);
  }
  int Write(const uint8_t *writeBuffer, uint32_t size) {
    return Write(address_, writeBuffer, size);
  }
  int Read(uint8_t *readBuffer, uint32_t size) {
    return Read(address_, readBuffer, size);
  }
  uint16_t Address() const { return address_; }
  void setAddress(uint16_t address) { address_ = address; }

  virtual int setOption(int option) { return -1; }
  virtual int applySetting() { return -1; }

private:
  uint16_t address_ = 0;
  uint32_t option_ = 0;
  bool isOpen_ = false;

public:
  uint32_t Option() const { return option_; }
  bool IsOpen() const { return isOpen_; }

protected:
  void setIsOpen(bool isOpen) { isOpen_ = isOpen; }
};
} // namespace gramsballoon::pgrams
#endif //GB_I2CInterface_hh