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

public:
  virtual int baudrate() const { return 0; }
  virtual void setBaudrate(unsigned int) {}
  virtual void setConfigOptions(unsigned int) {}
  virtual int Open(int) { return -1; }
  virtual int Close() { return -1; }
  virtual int WriteAfterRead(int, const uint8_t *, int, uint8_t *, int) { return -1; }
  virtual int WriteAndRead(int, uint8_t *, unsigned int, uint8_t *) { return -1; }
  virtual int Write(int, const uint8_t *, unsigned int) { return -1; }
  virtual int controlGPIO(int, bool) { return -1; }
};

} // namespace gramsballoon::pgrams

#endif /* SPIInterface_H */
