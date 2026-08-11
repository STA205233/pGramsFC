#ifndef GB_SPIInterfaceMultiplexer_hh
#define GB_SPIInterfaceMultiplexer_hh 1
#include "SPIInterface.hh"
#include "VCSMapping.hh"
#include <cstdint>
#include <memory>
#include <optional>
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
  std::unique_ptr<VCSMapping> csMapping_ = nullptr;
  std::shared_ptr<SPIInterface> baseInterface_ = nullptr;

public:
  void setBaseInterface(std::shared_ptr<SPIInterface> &baseInterface) { baseInterface_ = baseInterface; }
  void setMappingChipSelect(std::unique_ptr<VCSMapping> &&mapping) { csMapping_ = std::move(mapping); }
  std::optional<VCSMapping::pair_t> getMappingChipSelect(int multiplexerChannel) const;
  std::optional<VCSMapping::cs_t> getDefaultState() const {
    if (!csMapping_) { return std::nullopt; }
    return csMapping_->DefaultState();
  }

  int controlGPIO(int cs, bool value) override;
  int controlGPIOBit(uint32_t cs, uint32_t value) override;

  template <typename F>
  int executeFunction(int multiplexerChannel, bool csControl, F &&f);

  const std::vector<VCSMapping::pair_t> &Channels() const override {
    static const std::vector<VCSMapping::pair_t> empty;
    return csMapping_ ? csMapping_->Channels() : empty;
  }

  int Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) override;
  int WriteThenRead(int cs, const uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer, unsigned int rsize, bool csControl) override;
  int WriteAndRead(int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer, bool csControl) override;
  int Open(int channel, const char *path) override;
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

  void setConfigOptions(unsigned int options) override {
    if (baseInterface_) {
      baseInterface_->setConfigOptions(options);
    }
  }
  int MaximumCh() const override {
    return csMapping_ ? csMapping_->NumChannels() : 0;
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
    ret = baseInterface_->controlGPIOBit(static_cast<uint32_t>(mapped->first), static_cast<uint32_t>(mapped->second));
    if (ret != 0) {
      return ret;
    }
  }

  ret = std::forward<F>(f)(-1); // CS control is already handled above via controlGPIOBit; the base interface does not need a real channel.

  if (csControl) {
    const int releaseRet = baseInterface_->controlGPIOBit(static_cast<uint32_t>(mapped->first), getDefaultState().value());
    if (ret == 0) {
      ret = releaseRet;
    }
  }
  return ret;
}

} // namespace gramsballoon::pgrams
#endif // GB_SPIInterfaceMultiplexer_hh