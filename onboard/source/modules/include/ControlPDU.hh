#ifndef GRAMSBalloon_ControlPDU_hh
#define GRAMSBalloon_ControlPDU_hh 1
#include "DAC121S101IO.hh"
#include "SPIManager.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
#include <cstdint>
namespace gramsballoon::pgrams {

/**
 * @brief A module to control PDU
 * @author Shota Arai
 * @date 2026-02-20 | Shota Arai | Created (v1.0)
 * @date 2026-07-11 | Shota Arai | Added setVoltage (v1.1)
 */
class ControlPDU: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(ControlPDU, 1.1);
  ENABLE_PARALLEL_RUN();

public:
  ControlPDU();
  virtual ~ControlPDU() = default;

protected:
  ControlPDU(const ControlPDU& r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  int initializeDAC(int cs);
  int setVoltage(int cs, float voltage);
  int setVoltageRaw(int cs, uint16_t voltage);
  int controlGPIO(uint32_t cs, bool value);

private:
  SPIManager *spiManager_ = nullptr;
  std::string spiManagerName_ = "SPIManager";
  SendTelemetry *sendTelemetry_ = nullptr;
  std::shared_ptr<DAC121S101IO> dac_ = nullptr;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_ControlPDU_hh