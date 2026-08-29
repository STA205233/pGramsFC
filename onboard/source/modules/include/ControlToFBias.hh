#ifndef GB_ControlTofBias_hh
#define GB_ControlTofBias_hh 1
#include "anlnext/BasicModule.hh"
#include <chrono>
#include <memory>
namespace pgrams::communication {
enum class TelemetryCodes : uint16_t;
}

namespace gramsballoon::pgrams {
class ToFBiasController;
class SendTelemetry;
template <typename T>
class MosquittoManager;
class ToFBiasTelemetry;
class CommunicationFormat;

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

  int setVoffset(uint32_t voltage);
  int setTmuxChannel(uint32_t channel, int on_off);
  int setVdef(uint32_t channel, uint32_t voltage);
  int enableDCDC(uint32_t channel);
  int disableDCDC(uint32_t channel);
  int queryFullOutput();

private:
  SendTelemetry *sendTelemetry_ = nullptr;
  MosquittoManager<std::string> *mosquittoManager_ = nullptr;
  std::string mosquittoManagerName_ = "TelemMosquittoManager";

  enum class FullOutputStatus {
    WAITING,
    REQUESTING,
    REQUESTED
  };

  FullOutputStatus fullPacketStatus_;

  std::shared_ptr<ToFBiasController> controller_;
  std::string path_;
  size_t index_;
  int timeout_;
  std::chrono::time_point<std::chrono::steady_clock> lastReceivedTime_;
  std::chrono::seconds duration_;
  int minDurationSec_ = 1;
  int chatter_ = 0;

  int qos_ = 0;
  std::string topic_ = "TB_Telemetry";
  std::string starlinkTopic_ = "TB_Telemetry_Starlink";
  std::shared_ptr<ToFBiasTelemetry> telem_ = nullptr;
  std::string telemetryStr_;

  void treatError();
  void sendPacket(const std::string &str, ::pgrams::communication::TelemetryCodes code);
};
} // namespace gramsballoon::pgrams
#endif // GB_ControlTofBias_hh