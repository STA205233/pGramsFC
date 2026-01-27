#ifndef GRAMSBalloon_DetectErrorCallbackFromDAQ_hh
#define GRAMSBalloon_DetectErrorCallbackFromDAQ_hh 1
#include "DividePacket.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
namespace gramsballoon::pgrams {
class DividePacket;
class SendTelemetry;
class DetectErrorCallbackFromDAQ: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(DetectErrorCallbackFromDAQ, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  DetectErrorCallbackFromDAQ() = default;
  virtual ~DetectErrorCallbackFromDAQ() = default;

protected:
  DetectErrorCallbackFromDAQ(const DetectErrorCallbackFromDAQ &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

private:
  std::string dividePacketModuleName_ = "DividePacket";
  const DividePacket *dividePacket_ = nullptr;
  int chatter_ = 0;
  SendTelemetry *sendTelemetry_ = nullptr;
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_DetectErrorCallbackFromDAQ_hh