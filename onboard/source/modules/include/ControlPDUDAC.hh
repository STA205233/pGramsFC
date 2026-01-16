#ifndef GRAMSBalloon_ControlPDUDAC_hh
#define GRAMSBalloon_ControlPDUDAC_hh 1
#include "DAC121S101IO.hh"
#include "SPIManager.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
namespace gramsballoon::pgrams {
class SPIManager;
class SendTelemetry;
class ControlPDUDAC: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(ControlPDUDAC, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  ControlPDUDAC() = default;
  virtual ~ControlPDUDAC() = default;

protected:
  ControlPDUDAC(const ControlPDUDAC &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;
  void setVoltage(int channel, float v) {
    channel_ = channel;
    voltage_ = v;
  }

private:
  std::shared_ptr<DAC121S101IO> dacInterface_ = nullptr;
  std::string spiManagerName_ = "SPIManager";
  SPIManager *spiManager_ = nullptr;
  SendTelemetry *sendTelemetry_ = nullptr;
  int channel_ = 0;
  double supplyVoltage_ = 3.3f;
  int chatter_ = 0;
  float voltage_ = 0.0f;
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_ControlPDUDAC_hh