#ifndef GB_I2CInterface_hh
#define GB_I2CInterface_hh 1
#include <cstdint>
namespace gramsballoon::pgrams {
class I2CInterface {
public:
  I2CInterface() = default;
  virtual ~I2CInterface() = default;

protected:
  I2CInterface(const I2CInterface &) = delete;

public:
  virtual int Open(int) { return -1; }
  virtual int Close() { return -1; }
  virtual int WriteThenRead(uint16_t address, const uint8_t *, int, uint8_t *, int) { return -1; }
  virtual int Write(uint16_t address, const uint8_t *, unsigned int) { return -1; }
  virtual int Read(uint16_t address, uint8_t *, unsigned int) { return -1; }
  virtual int setOption(int option) { return -1; }
  virtual int applySetting() { return -1; }

private:
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