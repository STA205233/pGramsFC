#ifndef GRAMSBalloon_BayCatControl_hh
#define GRAMSBalloon_BayCatControl_hh 1
#include "SPIInterface.hh"
#define linux 1
#include "stdbool.h" // This is needed for including VL_OSALib.h
#include "VL_OSALib.h"
#include <map>
#include <stdint.h>
namespace gramsballoon::pgrams {
class BayCatControl: public SPIInterface {
  static constexpr unsigned int SPI_MODE_MASK = 0x3;
  static constexpr unsigned int SPI_SHIFT_DIRECTION_MASK = 0x4;
  static constexpr unsigned int SPI_SHIFT_DIRECTION_OFFSET = 2;

  static constexpr unsigned long DIR_GPIO = 0xCA1;
  static constexpr unsigned long AUX_OUT = 0xCA3;

public:
  BayCatControl();
  virtual ~BayCatControl() = default;
  BayCatControl(const BayCatControl &) = delete;

private:
  bool isOpen_ = false;
  unsigned int baudrate_ = 0;
  unsigned int options_ = 0;
  std::map<int, unsigned int> baudrateList_;

public:
  int baudrate() const override { return baudrate_; }
  void setBaudrate(unsigned int baudrate) override;
  void setConfigOptions(unsigned int configOptions) override { options_ = configOptions; }
  int updateSetting();
  int Open(int channel) override;
  int Close() override;
  int WriteAfterRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) override;
  int WriteAndRead(int /*cs*/, uint8_t * /*writeBuffer*/, unsigned int /*size*/, uint8_t * /*readBuffer*/) override { return -1; }
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size) override;
  int controlGPIO(int cs, bool value);
  int controlDIO(int cs, bool value);
  int controlFPGAGPIO(int cs, bool value);
  int WriteFPGARegister(unsigned long reg, unsigned char data);
  int WriteFPGARegisterOneChannel(unsigned long reg, int gpioId, bool data);
  int ReadFPGARegister(unsigned long reg, unsigned char *data);
  int ReadFPGARegisterOneChannel(unsigned long reg, int gpioId, bool *value);
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_BayCatSPIInterface_hh