#ifndef SPIInterface_H
#define SPIInterface_H 1

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
/**
 * A class of SPI Interface
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 * @date 2025-05-02 | Shota Arai | Modified to use the FT232H, major refactoring
 */
namespace gramsballoon::pgrams {
class SPIInterface {
public:
  SPIInterface() = default;
  virtual ~SPIInterface() = default;

protected:
  SPIInterface(const SPIInterface &) = delete;
  void setIsOpen(bool isOpen) { isOpen_ = isOpen; }

public:
  unsigned int Baudrate() const { return baudrate_; }
  virtual int updateSetting() { return -1; }
  virtual void setBaudrate(unsigned int baudrate) {
    baudrate_ = baudrate;
  }
  bool IsOpen() const { return isOpen_; }
  void setConfigOptions(unsigned int configOptions) { configOptions_ = configOptions; }
  unsigned int ConfigOptions() const { return configOptions_; }
  virtual int Open(int) { return -1; }
  virtual int Close() { return -1; }
  virtual int WriteThenRead(int, const uint8_t *, int, uint8_t *, int, bool = true) { return -1; }
  virtual int WriteAndRead(int, uint8_t *, unsigned int, uint8_t *, bool = true) { return -1; }
  virtual int Write(int, const uint8_t *, unsigned int, bool = true) { return -1; }
  virtual int controlGPIO(int, bool) { return -1; }
  virtual int MaximumCh() { return 0; }

private:
  unsigned int baudrate_ = 1000000;
  unsigned int configOptions_ = 0;
  bool isOpen_ = false;
};

} // namespace gramsballoon::pgrams

#endif /* SPIInterface_H */
