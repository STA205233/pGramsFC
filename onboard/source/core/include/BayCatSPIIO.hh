#include <sys/types.h>
#ifndef GRAMSBalloon_BayCatControl_hh
#define GRAMSBalloon_BayCatControl_hh 1
#include "BayCatAPICaller.hh"
#include "SPIInterface.hh"
#include <cstdint>
#include <map>
namespace gramsballoon::pgrams {
/**
 * @brief A class to control the SPI interface of BayCat.
 * @author Shota Arai
 * @date 2025-**-** | Shota Arai | Created
 * @date 2026-04-10 | Shota Arai | Refactored to use BayCatAPICaller
 * @date 2026-07-10 | Shota Arai | Added controlGPIOBit
 */
class BayCatSPIIO: public SPIInterface, public BayCatAPICaller {
public:
  static constexpr unsigned int SPI_MODE_MASK = 0x3;
  static constexpr unsigned int SPI_SHIFT_DIRECTION_MASK = 0x4;
  static constexpr unsigned int SPI_SHIFT_DIRECTION_OFFSET = 2;
  static unsigned int MakeOption(unsigned int mode, unsigned int shiftDirection) {
    return (mode & SPI_MODE_MASK) | ((shiftDirection << SPI_SHIFT_DIRECTION_OFFSET) & SPI_SHIFT_DIRECTION_MASK);
  }

  static constexpr unsigned long DIR_GPIO = 0x21;
  static constexpr unsigned long AUX_OUT = 0x23;
  static constexpr unsigned long AUX_IN = 0x24;

public:
  BayCatSPIIO();
  virtual ~BayCatSPIIO() = default;
  BayCatSPIIO(const BayCatSPIIO&) = delete;
  int MaximumCh() override { return 24; }
  int setSpiMode(int mode) override {
    return SPISetMode(static_cast<unsigned int>(mode));
  }

private:
  std::map<int, unsigned int> baudrateList_;

public:
  int updateSetting() override;
  void setBaudrate(unsigned int baudrate) override;
  int Open(int channel) override;
  int Close() override;
  int WriteThenRead(int cs, const uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer, unsigned int rsize, bool csControl) override;
  int WriteAndRead(int /*cs*/, uint8_t * /*writeBuffer*/, unsigned int /*size*/, uint8_t * /*readBuffer*/, bool csControl) override;
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) override;
  int controlGPIO(int cs, bool value) override;
  int controlGPIOBit(uint32_t csBit, uint32_t value) override;
  int WriteFPGARegister(unsigned long reg, unsigned char data);
  int WriteFPGARegisterMultiChannel(unsigned long reg, uint32_t bitexpression, bool data);
  int WriteFPGARegisterMultiChannel(unsigned long reg, uint32_t bitexpression, uint32_t data);
  int ReadFPGARegister(unsigned long reg, unsigned char *data);

private:
  int applyBaudrateSetting();
  int controlFPGAGPIO(uint32_t bitExpression, uint32_t value);
  int controlDIO(int cs, bool value);
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_BayCatSPIInterface_hh