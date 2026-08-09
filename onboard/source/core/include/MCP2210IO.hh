#ifndef GB_MCP2210IO_hh
#define GB_MCP2210IO_hh 1
#include "SPIInterface.hh"
namespace gramsballoon::pgrams {
class MCP2210IO final: public SPIInterface {
public:
  MCP2210IO();
  virtual ~MCP2210IO();
  static unsigned int MakeOption(unsigned int mode) {
    return mode;
  }

private:
  int handler_;
  int spiMode_;
  int speed_;

public:
  int updateSetting() override;
  void setConfigOptions(unsigned int configOptions) override;
  int WriteThenRead(int, const uint8_t *, unsigned int, uint8_t *, unsigned int, bool = true) override;
  int WriteAndRead(int, uint8_t *, unsigned int, uint8_t *, bool) override;
  int Open(int channel, const char *path) override;
  int controlGPIO(int cs, bool value) override;
  int Write(int, const uint8_t *, unsigned int, bool = true) override;
  int controlGPIOBit(uint32_t, uint32_t) override;
  int MaximumCh();
};
} // namespace gramsballoon::pgrams
#endif //GB_MCP2210IO_hh