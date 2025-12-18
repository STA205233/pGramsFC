#ifndef GB_SendArrayByMQTT_hh
#define GB_SendArrayByMQTT_hh 1
#include "InterpretTelemetry.hh"
#include "MosquittoManager.hh"
#include "anlnext/BasicModule.hh"
#include <string>
#include <vector>
namespace gramsballoon::pgrams {
/**
 * @brief Send array interpreted from telemetry to the other subsystem ground software
 * @author Shota Arai
 * @date 2025-12-18 | First created (v.1.0)
 */
class SendArrayByMQTT: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(SendArrayByMQTT, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  SendArrayByMQTT() = default;
  virtual ~SendArrayByMQTT() = default;

protected:
  SendArrayByMQTT(const SendArrayByMQTT &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

private:
  MosquittoManager<std::vector<uint8_t>> *mosquittoManager_ = nullptr;
  std::string mosquittoManagerName_ = "MosquittoManager";
  const InterpretTelemetry *interpretTelemetry_ = nullptr;
  std::string interpretTelemetryName_ = "InterpretTelemetry";
  std::vector<uint8_t> data_;

  std::string topic_ = "Telemetry_array";
  int qos_ = 0;
};
} // namespace gramsballoon::pgrams
#endif // GB_SendArrayByMQTT_hh