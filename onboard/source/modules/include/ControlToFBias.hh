#ifndef GB_ControlTofBias_hh
#define GB_ControlTofBias_hh 1
#include "anlnext/BasicModule.hh"
#include <chrono>
#include <memory>
#include <string>

namespace gramsballoon::pgrams {
class ToFBiasController;
class SendTelemetry;

class ControlToFBias: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(ControlToFBias, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  ControlToFBias();
  virtual ~ControlToFBias() = default;

protected:
  ControlToFBias(const ControlToFBias &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_begin_run() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_end_run() override;
  anlnext::ANLStatus mod_finalize() override;

  int setTrimVoltage(uint32_t channel0, uint32_t channle1, uint32_t channle2, uint32_t channel3, uint32_t voltage);
  int setDefaultTemperature(uint32_t channel, int temperature);
  int enableTemperatureCon();
  int disableTemperatureCon();
  int enableDCDC(uint8_t channel);
  int disableDCDC(uint8_t channel);

private:
  SendTelemetry *sendTelemetry_;
  std::shared_ptr<ToFBiasController> controller_;
  std::string path_;
  std::array<uint16_t, 128> adcData_;
  std::array<uint16_t, 32> tempData_;
  std::array<uint16_t, 128> dacData_;
  size_t lastIndex_;
  std::chrono::time_point<std::chrono::steady_clock> lastReceivedTime_;
  std::chrono::seconds duration_;
  int minDurationSec_ = 1;
  int chatter_ = 0;
};
} // namespace gramsballoon::pgrams
#endif // GB_ControlTofBias_hh