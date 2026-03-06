#ifndef GB_SendPacketByMQTT_hh
#define GB_SendPacketByMQTT_hh 1
#include "InterpretTelemetry.hh"
#include "MosquittoManager.hh"
#include "anlnext/BasicModule.hh"

namespace gramsballoon::pgrams {

class InterpretTelemetry;

class SendPacketByMQTT: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(SendPacketByMQTT, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  SendPacketByMQTT() = default;
  virtual ~SendPacketByMQTT() = default;

protected:
  SendPacketByMQTT(const SendPacketByMQTT &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

private:
  const InterpretTelemetry *interpretTelemetry_ = nullptr;
  std::string interpretTelemetryName_ = "InterpretTelemetry";
  std::string mosquittoManagerName_ = "MosquittoManager";
  MosquittoManager<std::string> *mosquittoManager_ = nullptr;
  std::string topic_ = "pgrams/telemetry";
  int qos_ = 0;
  int chatter_ = 0;
  std::string data_;
};
} // namespace gramsballoon::pgrams
#endif // GB_SendPacketByMQTT_hh