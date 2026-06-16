#ifndef GB_ConvertHubHKTelemetry_hh
#define GB_ConvertHubHKTelemetry_hh 1
#include "InterpretTelemetry.hh"
#include "anlnext/BasicModule.hh"
#include <string>
#include <memory>
namespace gramsballoon::pgrams {
class ConvertedHubHKTelemetry;
class HubHKTelemetry;


/**
 * @brief Convert received HubHKTelemetry
 * @author Shota Arai
 * @date 2026-06-16 | Shota Arai | First version (v1.0)
 */
class ConvertHubHKTelemetry: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(ConvertHubHKTelemetry, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  ConvertHubHKTelemetry() = default;
  virtual ~ConvertHubHKTelemetry() = default;

protected:
  ConvertHubHKTelemetry(const ConvertHubHKTelemetry &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

private:
  const InterpretTelemetry *interpretTelemetry_ = nullptr;
  std::shared_ptr<HubHKTelemetry> rawTelemetry_ = nullptr;
  std::string HubHKInterpreterName_ = "InterpretTelemetry";
  int verbose_ = 0;
  std::shared_ptr<ConvertedHubHKTelemetry> convertedTelemetry_ = nullptr;
};
} // namespace gramsballoon::pgrams
#endif // GB_ConvertHubHKTelemetry_hh