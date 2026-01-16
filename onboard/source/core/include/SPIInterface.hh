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
  virtual int WriteThenRead(int, const uint8_t *, int, uint8_t *, int) { return -1; }
  virtual int WriteAndRead(int, uint8_t *, unsigned int, uint8_t *) { return -1; }
  virtual int Write(int, const uint8_t *, unsigned int) { return -1; }
  virtual int controlGPIO(int, bool) { return -1; }

  // Multiplexer versions
  virtual int controlGPIOMul(const uint32_t /*state*/) { return -1; }
  int WriteAndReadMul(int channel, const uint8_t *dataToWrite, int writeSize, uint8_t *dataToRead, int readSize) {
    return multiplexerControl(channel, &SPIInterface::WriteThenRead, dataToWrite, writeSize, dataToRead, readSize);
  }
  int WriteMul(int channel, const uint8_t *dataToWrite, unsigned int size) {
    return multiplexerControl(channel, &SPIInterface::Write, dataToWrite, size);
  }
  int WriteThenReadMul(int channel, const uint8_t *dataToWrite, int writeSize, uint8_t *dataToRead, int readSize) {
    return multiplexerControl(channel, &SPIInterface::WriteThenRead, dataToWrite, writeSize, dataToRead, readSize);
  }

private:
  unsigned int baudrate_ = 1000000;
  unsigned int configOptions_ = 0;
  bool isOpen_ = false;
  template <typename T, typename... Args>
  int multiplexerControl(int channel, T func, Args... args) {
    if (channel == 0) {
      return -1;
    }
    controlGPIOMul(channel);
    return (this->*func)(channel, args...);
  }
};

} // namespace gramsballoon::pgrams

#endif /* SPIInterface_H */
