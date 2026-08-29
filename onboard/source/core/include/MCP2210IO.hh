#ifndef GB_MCP2210IO_hh
#define GB_MCP2210IO_hh 1
#include "SPIInterface.hh"
#include <array>
#include <cstdint>
namespace gramsballoon::pgrams {

/**
 * @brief An IO class for MCP2210.
 * @note This function is not thread-safe.
 * @author Shota Arai
 * @date 2026-08-11 | Shota Arai | Created
 * @date 2026-08-21 | Shota Arai | Finished implementation
 */
class MCP2210IO final: public SPIInterface {
public:
  MCP2210IO();
  virtual ~MCP2210IO();
  static unsigned int MakeOption(unsigned int mode) {
    return mode;
  }

private:
  static constexpr int ALL_HIGH = ~0;
  int handler_;
  int spiMode_;
  static constexpr size_t NUM_BUF = 128;
  std::array<uint8_t, NUM_BUF> wbuffer_;
  std::array<uint8_t, NUM_BUF> rbuffer_;

public:
  int updateSetting() override { return 0; };
  void setConfigOptions(unsigned int configOptions) override;
  int WriteThenRead(int, const uint8_t *, unsigned int, uint8_t *, unsigned int, bool = true) override;
  int WriteAndRead(int, uint8_t *, unsigned int, uint8_t *, bool) override;
  int Open(int channel, const char *path) override;
  int Close() override;
  int controlGPIO(int cs, bool value) override;
  int Write(int, const uint8_t *, unsigned int, bool = true) override;
  int controlGPIOBit(uint32_t, uint32_t) override;
  int MaximumCh() const override;
};
} // namespace gramsballoon::pgrams
#endif //GB_MCP2210IO_hh