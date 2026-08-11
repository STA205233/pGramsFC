#ifndef GRAMSBalloon_FT232HIO_hh
#define GRAMSBalloon_FT232HIO_hh 1
#include "MPSSEController.hh"
#include "MPSSEDeviceManager.hh"
#include "SPIInterface.hh"
#include <cstdint>
#include <memory>
namespace gramsballoon::pgrams {
/**
 * @brief Control Class for FT232H
 * @author Shota Arai
 * @date 2025-**-** | Shota Arai | First version
 * @date 2026-07-10 | Shota Arai | Added controlGPIOBit
 */
class FT232HIO: public SPIInterface {
private:
public:
  FT232HIO();
  virtual ~FT232HIO() = default;
  int MaximumCh() const override { return 8; }

protected:
  FT232HIO(const FT232HIO &) = delete;

public:
  struct config {
    static constexpr unsigned int SPI_MODE0 = 0x0;
    static constexpr unsigned int SPI_MODE1 = 0x1;
    static constexpr unsigned int SPI_MODE2 = 0x2;
    static constexpr unsigned int SPI_MODE3 = 0x3;
  };
  int Open(int channel, const char *) override;
  int Close() override {
    if (!IsOpen()) {
      return 0;
    }
    if (mpsseController_) {
      return mpsseController_->close();
    }
    setIsOpen(false);
    return 0;
  }
  int WriteThenRead(int cs, const uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer, unsigned int rsize, bool csControl) override;
  int WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer, bool csControl) override;
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) override;
  int controlGPIO(int cs, bool value) override;
  int controlGPIOBit(uint32_t cs, uint32_t value) override;
  int updateSetting() override;

private:
  std::shared_ptr<mpsse::MPSSEDeviceManager> mpsseDeviceManager_;
  std::shared_ptr<mpsse::MPSSEController> mpsseController_;
  std::vector<uint8_t> writeBuffer_;
  std::vector<uint8_t> readBuffer_;
};

} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_FT232HIO_hh