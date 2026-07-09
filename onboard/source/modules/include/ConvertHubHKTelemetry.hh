#ifndef GB_ConvertHubHKTelemetry_hh
#define GB_ConvertHubHKTelemetry_hh 1
#include "ConvertedHubHKTelemetry.hh"
#include "InterpretTelemetry.hh"
#include "VDBDataStore.hh"
#include "anlnext/BasicModule.hh"
#include <memory>
#include <string>
namespace gramsballoon::pgrams {
class ConvertedHubHKTelemetry;
class HubHKTelemetry;

/**
 * @brief Convert received HubHKTelemetry
 * @author Shota Arai
 * @date 2026-06-16 | Shota Arai | First version (v1.0)
 * @date 2026-07-09 | Shota Arai | Inherited from VDBDataStore
 */
class ConvertHubHKTelemetry: public anlnext::BasicModule, VDBDataStore {
  DEFINE_ANL_MODULE(ConvertHubHKTelemetry, 1.1);
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

  void pushToDBSink(DBFieldSink *sink) const override {
    if (singleton_self()->convertedTelemetry_) {
      singleton_self()->convertedTelemetry_->serialize(sink);
    }
  }

  void initializeDBTableInSink(DBFieldSink *sink, const std::string &table_name) const override {
    if (singleton_self()->convertedTelemetry_) {
      singleton_self()->convertedTelemetry_->initializeDBTable(sink, table_name);
    }
  }
  bool hasData() const override { return interpretTelemetry_->hasData(); }

private:
  const InterpretTelemetry *interpretTelemetry_ = nullptr;
  std::shared_ptr<HubHKTelemetry> rawTelemetry_ = nullptr;
  std::string HubHKInterpreterName_ = "InterpretTelemetry";
  int verbose_ = 0;
  std::shared_ptr<ConvertedHubHKTelemetry> convertedTelemetry_ = nullptr;
};
} // namespace gramsballoon::pgrams
#endif // GB_ConvertHubHKTelemetry_hh