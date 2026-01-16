#ifndef GRAMSBalloon_FT232HIO_hh
#define GRAMSBalloon_FT232HIO_hh 1
#include "MPSSEController.hh"
#include "MPSSEDeviceManager.hh"
#include "SPIInterface.hh"
#include "ftd2xx.h"
namespace gramsballoon::pgrams {
class FT232HIO: public SPIInterface {
private:
public:
  FT232HIO();
  virtual ~FT232HIO() = default;

protected:
  FT232HIO(const SPIInterface &) = delete;

public:
  struct config {
    static constexpr unsigned int SPI_MODE0 = 0x0;
    static constexpr unsigned int SPI_MODE1 = 0x1;
    static constexpr unsigned int SPI_MODE2 = 0x2;
    static constexpr unsigned int SPI_MODE3 = 0x3;
  };
  int Open(int channel) override;
  int Close() override {
    if (!IsOpen()) {
      return 0;
    }
    if (mpsseController_) {
      return mpsseController_->close();
    }
    return 0;
  }
  int WriteThenRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) override;
  int WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer) override;
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size) override;
  int controlGPIO(int cs, bool value) override;
  int updateSetting() override;

private:
  std::shared_ptr<mpsse::MPSSEDeviceManager> mpsseDeviceManager_;
  std::shared_ptr<mpsse::MPSSEController> mpsseController_;
  unsigned int configOptions_ = 0;
  std::vector<uint8_t> writeBuffer_;
  std::vector<uint8_t> readBuffer_;
};

} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_FT232HIO_hh