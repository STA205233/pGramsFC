#ifndef GB_GetLabJackData_hh
#define GB_GetLabJackData_hh 1
#include "LabJackIO.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
namespace gramsballoon::pgrams {
/**
  * @brief A module to get data from a LabJack device.
  * @author Shota Arai
  * @date 2026-04-04  | First version
  */

class SendTelemetry;
class GetLabJackData: public anlnext::BasicModule {
  using float32 = LabJackAddresses::float32;
  DEFINE_ANL_MODULE(GetLabJackData, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  GetLabJackData() = default;
  virtual ~GetLabJackData() = default;

protected:
  GetLabJackData(const GetLabJackData &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

private:
  std::shared_ptr<LabJackIO> labjackIO_ = nullptr;
  std::array<float32, 2> analogIn_ = {0.0, 0.0};
  float32 temperatureDevice_ = 0.0;
  float32 temperatureAir_ = 0.0;
  int chatter_ = 0;
  SendTelemetry *sendTelemetry_ = nullptr;
  double rangeSetting_ = 0.1;

public:
  const std::array<float32, 2> &getAnalogIn() const { return singleton_self()->analogIn_; }
  float32 getTemperatureDevice() const { return singleton_self()->temperatureDevice_; }
  float32 getTemperatureAir() const { return singleton_self()->temperatureAir_; }
};
} // namespace gramsballoon::pgrams
#endif // GB_GetLabJackData_hh