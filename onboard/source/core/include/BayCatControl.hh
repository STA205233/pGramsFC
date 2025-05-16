#ifndef GRAMSBalloon_BayCatControl_hh
#define GRAMSBalloon_BayCatControl_hh 1
#include "SPIInterface.hh"
#include "VL_OSALib.h"
#include <map>
#include <stdint.h>
namespace gramsballoon::pgrams {
class BayCatControl: public SPIInterface {
  static constexpr unsigned int SPI_MODE_MASK = 0x3;
  static constexpr unsigned int SPI_SHIFT_DIRECTION = 0x4;

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
private:
  int baudrate() const override { return baudrate_; }
  void setBaudrate(unsigned int baudrate) override;
  void setConfigOptions(unsigned int configOptions) override { options_ = configOptions; }
  int updateSetting();
  int Open(int channel) override;
  int Close() override;
  int WriteAfterRead(int cs, const uint8_t *writeBuffer, int wsize, uin / t8_t * readBuffer, int rsize) override;
  int WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer) override;
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size) override;
  int controlGPIO(int cs, bool value);
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_BayCatSPIInterface_hh