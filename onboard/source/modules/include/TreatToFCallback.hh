#ifndef GRAMSBalloon_TreatToFCallback_hh
#define GRAMSBalloon_TreatToFCallback_hh 1
#include "InterpretTelemetry.hh"
#include "MySQLFieldSink.hh"
#include "MySQLManager.hh"
#include "anlnext/BasicModule.hh"
#include <cstdint>
namespace gramsballoon::pgrams {
class InterpretTelemetry;

/**
 * Module for treating ToF callback data and storing it into MySQL database.
 *
 * @author Shota Arai
 * @date 2026-02-03 | Shota Arai | First design
 */
class TreatToFCallback: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(TreatToFCallback, 1.0);
  ENABLE_PARALLEL_RUN();

  enum class TOF_STATUS : uint32_t {
    SUCCESS = 0,
    FAILED = 1,
    INTERRUPTED = 2,
    UNKNOWN = 100,
  };

public:
  TreatToFCallback() = default;
  virtual ~TreatToFCallback() = default;

protected:
  TreatToFCallback(const TreatToFCallback &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

private:
  int chatter_ = 0;
  std::string tableName_ = "HubHK";
  MySQLManager *mysqlManager_ = nullptr;
  MySQLFieldSink mysqlFieldSink_;
  const InterpretTelemetry *interpretTelemetry_ = nullptr;
  std::string interpretTelemetryName_ = "InterpretTelemetry";
  uint8_t statusCode_;
  TOF_STATUS status_;
  uint16_t command_;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_TreatToFCallback_hh