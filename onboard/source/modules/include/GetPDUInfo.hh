#ifndef GRAMSBalloon_GetPDUInfo_hh
#define GRAMSBalloon_GetPDUInfo_hh 1
#include "ADC128S102IO.hh"
#include "SPIManager.hh"
#include "anlnext/BasicModule.hh"
#include <memory>
#include <string>
namespace gramsballoon::pgrams {
class SPIManager;

/**
 * @brief A module to get PDU information
 * @author Shota Arai
 * @date 2026-02-20 | Shota Arai | Created (v1.0)
 */
class GetPDUInfo: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(GetPDUInfo, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  GetPDUInfo() = default;
  virtual ~GetPDUInfo() = default;

protected:
  GetPDUInfo(const GetPDUInfo &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

private:
  SPIManager *spiManager_ = nullptr;
  std::string spiManagerName_;
  int numAdcs_ = 8;
  std::shared_ptr<ADC128S102IO> adc_ = nullptr;
  SendTelemetry *sendTelemetry_ = nullptr;
  std::vector<float> voltages_;
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_GetPDUInfo_hh