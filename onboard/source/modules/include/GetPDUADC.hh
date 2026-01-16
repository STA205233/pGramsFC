#ifndef GRAMSBalloon_GetPDUADC_hh
#define GRAMSBalloon_GetPDUADC_hh 1
#include "ADC128S102IO.hh"
#include "SPIManager.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
#include <memory>
#include <string>
namespace gramsballoon::pgrams {
class SPIManager;
class SendTelemetry;

class GetPDUADC: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(GetPDUADC, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  GetPDUADC() = default;
  virtual ~GetPDUADC() = default;

protected:
  GetPDUADC(const GetPDUADC &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

private:
  std::shared_ptr<ADC128S102IO> adcInterface_ = nullptr;
  SPIManager *spiManager_ = nullptr;
  std::string spiManagerName_ = "SPIManager";
  SendTelemetry *sendTelemetry_ = nullptr;
  int chatter_ = 0;
  int channel_ = 0;
  double supplyVoltage_ = 3.3f;
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_GetPDUADC_hh