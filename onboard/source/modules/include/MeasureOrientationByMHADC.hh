#ifndef GB_MeasureOrientationByMHADC_hh
#define GB_MeasureOrientationByMHADC_hh 1
#include "EncodedSerialCommunicator.hh"
#include "OrientationInformation.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
#include <regex>
#include <string>
namespace gramsballoon {
class SendTelemetry;
}
namespace gramsballoon::pgrams {
class MeasureOrientationByMHADC: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(MeasureOrientationByMHADC, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  MeasureOrientationByMHADC() = default;
  virtual ~MeasureOrientationByMHADC() = default;

protected:
  MeasureOrientationByMHADC(const MeasureOrientationByMHADC &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;
  const OrientationInformation *GetOrientationInformation() { return orientationInfo_.get(); }

private:
  EncodedSerialCommunicator *esc_ = nullptr;
  SendTelemetry *sendTelemetry_ = nullptr;
  std::string encodedSerialCommunicatorName_ = "EncodedSerialCommunicator";
  std::shared_ptr<OrientationInformation> orientationInfo_ = nullptr;
  int sleepForMsec_ = 500;
  int numTrials_ = 1;
  int chatter_ = 0;
};
} // namespace gramsballoon::pgrams
#endif //GB_MeasureOrientationByMHADC_hh