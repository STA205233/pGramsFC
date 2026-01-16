#ifndef GRAMSBalloon_MeasureTemperatureWithRTDSensorByMHADC_hh
#define GRAMSBalloon_MeasureTemperatureWithRTDSensorByMHADC_hh 1
#ifdef NOT_USED_MODULE
#include "GetMHADCData.hh"
#include "MeasureTemperatureWithRTDSensor.hh"
#include "SendTelemetry.hh"
namespace gramsballoon::pgrams {
class SendTelemetry;

class MeasureTemperatureWithRTDSensorByMHADC: public MeasureTemperatureWithRTDSensor {
  DEFINE_ANL_MODULE(MeasureTemperatureWithRTDSensorByMHADC, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  MeasureTemperatureWithRTDSensorByMHADC() = default;
  virtual ~MeasureTemperatureWithRTDSensorByMHADC() = default;

protected:
  MeasureTemperatureWithRTDSensorByMHADC(const MeasureTemperatureWithRTDSensorByMHADC &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

private:
  GetMHADCData *getMHADCData_ = nullptr;
  std::string getMHADCDataName_ = "GetMHADCData";
  SendTelemetry *sendTelemetry_ = nullptr;
  int channel_ = 0;
  float ConvertTemperature(int adc, int bits, int offset) const;
  int bit_ = 10;
  int offset_ = 0;
  int chatter_ = 0;
  bool hasProblem_ = false;
};
} // namespace gramsballoon::pgrams
#endif // NOT_USED_MODULE
#endif //GRAMSBalloon_MeasureTemperatureWithRTDSensorByMHADC_hh