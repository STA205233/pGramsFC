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
 */
class SPIInterfaceMultiplexer {
public:
  SPIInterfaceMultiplexer() = default;
  virtual ~SPIInterfaceMultiplexer() = default;

protected:
  SPIInterfaceMultiplexer(const SPIInterfaceMultiplexer &) = delete;

private:
  std::vector<std::unique_ptr<SPIInterface>> spiInterfaces_;
  std::vector<std::vector<std::pair<int, int>>> csMapping_; // Pair: (interface index, cs). The vector index is (channel of multiplexer, bit position)

public:
  void addSPIInterface(const std::unique_ptr<SPIInterface> &spiInterface) {
    spiInterfaces_.push_back(std::move(spiInterface));
  }
  SPIInterface *getSPIInterface(size_t index) {
    if (index < spiInterfaces_.size()) {
      return spiInterfaces_[index].get();
    }
    return nullptr;
  }
  int baudrate(size_t index) const {
    if (index < spiInterfaces_.size()) {
      return spiInterfaces_[index]->baudrate();
    }
    return -1;
  }

  void setBaudrate(size_t index, unsigned int baudrate) {
    if (index < spiInterfaces_.size()) {
      spiInterfaces_[index]->setBaudrate(baudrate);
    }
  }

  void setConfigOptions(size_t index, unsigned int options) {
    if (index < spiInterfaces_.size()) {
      spiInterfaces_[index]->setConfigOptions(options);
    }
  }

  int Open(size_t index, int ch = 0) {
    if (index < spiInterfaces_.size()) {
      return spiInterfaces_[index]->Open(ch);
    }
    return -1;
  }

  int Close(size_t index) {
    if (index < spiInterfaces_.size()) {
      return spiInterfaces_[index]->Close();
    }
    return -1;
  }

  size_t getNumberOfInterfaces() const {
    return spiInterfaces_.size();
  }

  void setMappingChipSelect(int multiplexerChannel, int bitPosition, size_t interfaceIndex, int cs) {
    if (multiplexerChannel >= static_cast<int>(csMapping_.size())) {
      csMapping_.resize(multiplexerChannel + 1);
    }
    csMapping_[multiplexerChannel][bitPosition] = std::make_pair(static_cast<int>(interfaceIndex), cs);
  }

  std::pair<int, int> getMappingChipSelect(int multiplexerChannel, int bitPosition) const {
    if (multiplexerChannel < static_cast<int>(csMapping_.size()) && bitPosition < static_cast<int>(csMapping_[multiplexerChannel].size())) {
      return csMapping_[multiplexerChannel][bitPosition];
    }
    return std::make_pair(-1, -1);
  }
  template <typename Func, typename... Args>
  int executeFunction(int multiplexerChannel, int cs, Func SPIInterface::*func, Args... args) {
    bool success = false;
    for (int bit = 0; bit < 32; ++bit) {
      if (cs & (1 << bit)) {
        const auto [interfaceIndex, cs] = getMappingChipSelect(multiplexerChannel, bit);
        if (interfaceIndex >= 0 && cs >= 0) {
          SPIInterface *spiInterface = getSPIInterface(interfaceIndex);
          if (spiInterface != nullptr) {
            success &= (spiInterface->*func)(cs, args...);
          }
          else {
            success = false;
          }
        }
        return -1;
      }
    }
    return -1;
  }
};
} // namespace gramsballoon::pgrams
#endif //GB_SPIInterfaceMultiplexer_hh