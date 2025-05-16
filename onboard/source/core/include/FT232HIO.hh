#ifndef GRAMSBalloon_FT232HIO_hh
#define GRAMSBalloon_FT232HIO_hh 1
#include "SPIInterface.hh"
#include "WinTypes.h"
#include "ftd2xx.h"
#include "libmpsse_spi.h"
namespace gramsballoon::pgrams {
class FT232HIO: public SPIInterface {
private:
public:
  FT232HIO() = default;
  virtual ~FT232HIO() = default;

protected:
  FT232HIO(const SPIInterface &) = delete;
  FT_HANDLE SPIHandler() { return SPIHandler_; }

public:
  int baudrate() const override { return channelConfig_.ClockRate; }
  void setBaudrate(unsigned int baudrate) override { channelConfig_.ClockRate = baudrate; }
  void setConfigOptions(unsigned int configOptions) override { channelConfig_.configOptions = configOptions; }
  int Open(int channel) override;
  int Close() override {
    const FT_STATUS status = SPI_CloseChannel(SPIHandler_);
    if (status != FT_OK) {
      std::cerr << "SPI_CloseChannel failed: status = " << status << std::endl;
    }
    return -static_cast<int>(status);
  }
  int WriteAfterRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) override;
  int WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer) override;
  int Write(int cs, const uint8_t *writeBuffer, unsigned int size) override;
  int controlGPIO(int cs, bool value) override;
  int readGPIO(int cs) {
    UCHAR cs_;
    const FT_STATUS status = FT_ReadGPIO(SPIHandler_, &cs_);
    if (status != FT_OK) {
      std::cerr << "FT_ReadGPIO failed: status = " << status << std::endl;
      return -static_cast<int>(status);
    }
    return cs_ & (1 << static_cast<uint8_t>(cs)) ? 1 : 0;
  }
  int readGPIO() {
    UCHAR cs_;
    const FT_STATUS status = FT_ReadGPIO(SPIHandler_, &cs_);
    if (status != FT_OK) {
      std::cerr << "FT_ReadGPIO failed: status = " << status << std::endl;
      return -static_cast<int>(status);
    }
    return cs_;
  }

private:
  static constexpr int SPI_INTERNAL_CS = 8; // Internal Chip Select
  FT_HANDLE SPIHandler_ = nullptr;
  ChannelConfig channelConfig_;
  static constexpr int TRANSFER_OPTION_INTERNAL_CS = SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE | SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE; // Using Internal Chip Select
  static constexpr int TRANSFER_OPTION_EXTERNAL_CS = SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES; // Using External Chip Select
  std::vector<uint8_t> writeBuffer_;
  std::vector<uint8_t> readBuffer_;
};

} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_FT232HIO_hh