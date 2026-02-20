#ifndef GB_SPIInterfaceMultiplexer_hh
#define GB_SPIInterfaceMultiplexer_hh 1
#include "SPIInterface.hh"
#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>

namespace gramsballoon::pgrams {
class SPIInterface;

/**
 * @brief A class of SPI Interface Multiplexer
 * @note This function is not thread-safe.
 * @author Shota Arai
 * @date 2025-10-27 | Shota Arai | Created
 * @date 2026-02-20 | Shota Arai | Refactor to use only one SPIInterface per multiplexer
 */

template <typename SPIInterfaceType>
class SPIInterfaceMultiplexer: public SPIInterfaceType {
public:
  SPIInterfaceMultiplexer() = default;
  virtual ~SPIInterfaceMultiplexer() = default;

protected:
  SPIInterfaceMultiplexer(const SPIInterfaceMultiplexer &) = delete;

private:
  std::vector<uint32_t> csMapping_; //The vector index is channel of multiplexer and value means which cs is enabled.

public:
  void setMappingChipSelect(int multiplexerChannel, uint32_t chipSelect) {
    if (multiplexerChannel >= static_cast<int>(csMapping_.size())) {
      csMapping_.resize(multiplexerChannel + 1);
    }
    if (chipSelect >= static_cast<int>(csMapping_[multiplexerChannel].size())) {
      csMapping_[multiplexerChannel].resize(chipSelect + 1, 1 << chipSelect);
    }
    csMapping_[multiplexerChannel] = chipSelect;
  }
  uint32_t getMappingChipSelect(int multiplexerChannel) const {
    if (multiplexerChannel < static_cast<int>(csMapping_.size())) {
      return csMapping_[multiplexerChannel];
    }
    return -1;
  }
  template <typename Func, typename... Args>
  int executeFunction(int multiplexerChannel, int cs, Func SPIInterface::*func, Args... args) {
    bool success = false;
    const auto enable_flag = csMapping_[multiplexerChannel];
    for (size_t i = 0; i < sizeof(uint32_t) * 8; i++) {
      if ((enable_flag & (1 << i)) != 0) {
        const int status = (this->*func)(cs, args...);
        if (status < 0) {
          std::cerr << "Error executing SPI function on channel " << multiplexerChannel << ", cs " << i << ": " << status << std::endl;
          return status;
        }
        success = true;
      }
    }
    return success ? 0 : -1;
  }
};
} // namespace gramsballoon::pgrams
#endif //GB_SPIInterfaceMultiplexer_hh