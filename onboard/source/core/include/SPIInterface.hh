#ifndef SPIInterface_H
#define SPIInterface_H 1

#include "WinTypes.h"
#include "ftd2xx.h"
#include "libmpsse_spi.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
/**
 * A class of SPI Interface
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 * @date 2025-05-02 | Shota Arai | Modified to use the FT232H, majar refactoring
 */
namespace gramsballoon::pgrams {
class SPIInterface {
public:
  SPIInterface() = default;
  virtual ~SPIInterface() = default;

protected:
  SPIInterface(const SPIInterface &) = delete;

public:
  int baudrate() const { return channelConfig_.ClockRate; }
  void setBaudrate(unsigned int baudrate) { channelConfig_.ClockRate = baudrate; }
  void setConfigOptions(unsigned int configOptions) { channelConfig_.configOptions = configOptions; }
  FT_HANDLE SPIHandler() { return SPIHandler_; }
  FT_STATUS Open(int channel);
  FT_STATUS Close() {
    const FT_STATUS status = SPI_CloseChannel(SPIHandler_);
    if (status != FT_OK) {
      std::cerr << "SPI_CloseChannel failed: status = " << status << std::endl;
    }
    return status;
  }
  FT_STATUS WriteAndRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize);
  FT_STATUS Write(int cs, const uint8_t *writeBuffer, int size);

private:
  static constexpr int SPI_INTERNAL_CS = 8; // Internal Chip Select
  FT_HANDLE SPIHandler_ = nullptr;
  ChannelConfig channelConfig_;
  static constexpr int TRANSFER_OPTION_INTERNAL_CS = SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE | SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE; // Using Internal Chip Select
  static constexpr int TRAMSFER_OPTION_EXTERNAL_CS = SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES; // Using External Chip Select
  std::vector<uint8_t> writeBuffer_;
  std::vector<uint8_t> readBuffer_;
};

} // namespace gramsballoon::pgrams

#endif /* SPIInterface_H */
