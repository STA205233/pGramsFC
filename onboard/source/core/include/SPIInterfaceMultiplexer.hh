#ifndef GB_SPIInterfaceMultiplexer_hh
#define GB_SPIInterfaceMultiplexer_hh 1
#include "SPIInterface.hh"
#include "VCSMapping.hh"
#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>

namespace gramsballoon::pgrams {
class SPIInterface;
class VCSMapping;

/**
 * @brief A class of SPI Interface Multiplexer
 * @note This function is not thread-safe.
 * @author Shota Arai
 * @date 2025-10-27 | Shota Arai | Created
 * @date 2026-02-20 | Shota Arai | Refactor to use only one SPIInterface per multiplexer
 */

class SPIInterfaceMultiplexer: public SPIInterface {
public:
  SPIInterfaceMultiplexer() = default;
  virtual ~SPIInterfaceMultiplexer() = default;

protected:
  SPIInterfaceMultiplexer(const SPIInterfaceMultiplexer &) = delete;

private:
  std::shared_ptr<VCSMapping> csMapping_ = nullptr;
  std::shared_ptr<SPIInterface> baseInterface_ = nullptr;

public:
  void setBaseInterface(std::shared_ptr<SPIInterface> &&baseInterface) { baseInterface_ = baseInterface; }

  void setMappingChipSelect(std::shared_ptr<VCSMapping> &mapping) { csMapping_ = mapping; }
  std::optional<uint32_t> getMappingChipSelect(int multiplexerChannel) const;

  int controlGPIO(int cs, bool value) override;

  template <typename F>
  int executeFunction(int multiplexerChannel, bool csControl, F &&f);

  int Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) override;
  int WriteThenRead(int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize, bool csControl) override;
  int WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer, bool csControl) override;
  int Open(int channel) override;
  int Close() override;
  int updateSetting() override {
    if (baseInterface_) {
      return baseInterface_->updateSetting();
    }
    return -1;
  }
  void setBaudrate(unsigned int baudrate) override {
    if (baseInterface_) {
      baseInterface_->setBaudrate(baudrate);
    }
  }
  int MaximumCh() override {
    if (baseInterface_) {
      return (1 << baseInterface_->MaximumCh());
    }
    return 0;
  }
};

template <typename F>
int SPIInterfaceMultiplexer::executeFunction(int multiplexerChannel, bool csControl, F &&f) {
  const auto mapped = getMappingChipSelect(multiplexerChannel);
  if (!mapped.has_value()) {
    return -1;
  }

  int ret = 0;
  if (csControl) {
    ret = baseInterface_->controlGPIO(static_cast<int>(*mapped), true);
    if (ret != 0) {
      return ret;
    }
  }

  ret = std::forward<F>(f)(static_cast<int>(*mapped));

  if (csControl) {
    const int releaseRet = baseInterface_->controlGPIO(static_cast<int>(*mapped), false);
    if (ret == 0) {
      ret = releaseRet;
    }
  }
  return ret;
}

} // namespace gramsballoon::pgrams
#endif //GB_SPIInterfaceMultiplexer_hh