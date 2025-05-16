#ifndef GRAMSBalloon_ADC128S102IO_HH
#define GRAMSBalloon_ADC128S102IO_HH 1
#include "SPIInterface.hh"
#include "cstdint"
#include <memory>
namespace gramsballoon::pgrams {
class ADC128S102IO {
public:
  ADC128S102IO(float supply_voltage_in_V) : LSB_(supply_voltage_in_V / 4096), halfLSB_(LSB_ * 0.5) {}
  virtual ~ADC128S102IO() = default;

private:
  std::shared_ptr<SPIInterface> spiInterface_ = nullptr;
  int cs_ = 0;
  float LSB_ = 3.3 / 4096;
  float halfLSB_ = LSB_ * 0.5;
  uint8_t readBuffer_[2] = {0, 0};
  uint8_t writeBuffer_[2] = {0, 0};

public:
  float convertVoltage(uint16_t value);
  float getCurrentVoltage(int ch);
  float convertVoltage(uint16_t value) const;
  void setSPIInterface(std::shared_ptr<SPIInterface> spiInterface) { spiInterface_ = spiInterface; }
  void setCS(int cs) { cs_ = cs; }
  int getCS() const { return cs_; }
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_ADC128S102IO_HH