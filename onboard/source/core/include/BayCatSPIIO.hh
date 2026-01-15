#ifndef GRAMSBalloon_BayCatControl_hh
#define GRAMSBalloon_BayCatControl_hh 1
#include "SPIInterface.hh"
#define linux 1
#include "VL_OSALib.h"
#include "stdbool.h" // This is needed for including VL_OSALib.h
#include <map>
#include <stdint.h>
namespace gramsballoon::pgrams {
class BayCatSPIIO: public SPIInterface {
  static constexpr unsigned int SPI_MODE_MASK = 0x3;
  static constexpr unsigned int SPI_SHIFT_DIRECTION_MASK = 0x4;
  static constexpr unsigned int SPI_SHIFT_DIRECTION_OFFSET = 2;

  static constexpr unsigned long DIR_GPIO = 0xCA1;
  static constexpr unsigned long AUX_OUT = 0xCA3;

public:
  BayCatSPIIO();
  virtual ~BayCatSPIIO() = default;
  BayCatSPIIO(const BayCatSPIIO &) = delete;

private:
  std::map<int, unsigned int> baudrateList_;
public:
  int updateSetting() override;
  void setBaudrate(unsigned int baudrate) override;
  int Open(int channel) override;
  int Close() override;
  int WriteAfterRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) override;
  int WriteAndRead(int /*cs*/, uint8_t * /*writeBuffer*/, unsigned int /*size*/, uint8_t * /*readBuffer*/) override { return -1; }
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size) override;
  int controlGPIO(int cs, bool value);
  int WriteFPGARegister(unsigned long reg, unsigned char data);
  int WriteFPGARegisterOneChannel(unsigned long reg, int gpioId, bool data);
  int ReadFPGARegister(unsigned long reg, unsigned char *data);
  int ReadFPGARegisterOneChannel(unsigned long reg, int gpioId, bool *value);

private:
  int applyBaudrateSetting();
  int controlFPGAGPIO(int cs, bool value);
  int controlDIO(int cs, bool value);
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_BayCatSPIInterface_hh