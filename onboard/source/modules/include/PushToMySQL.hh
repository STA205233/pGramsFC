#ifndef GRAMSBalloon_PushToMySQL_hh
#define GRAMSBalloon_PushToMySQL_hh 1
#include "InterpretTelemetry.hh"
#include "MySQLFieldSink.hh"
#include "MySQLManager.hh"
#include "anlnext/BasicModule.hh"
#include <string>
#include <vector>

namespace gramsballoon {
class InterpretTelemetry;
namespace pgrams {
class MySQLFieldSink;
/**
 * @brief Module for pushing telemetry data to MySQL database.
 * @author Shota Arai
 * @date 2025-**-** | First design
 * @date 2025-12-14 | Shota Arai | Refactoring (v2.0)
 * @date 2026-02-04 | Shota Arai | Use MySQLManager (v3.0)
 */
class PushToMySQL: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(PushToMySQL, 3.0);
  ENABLE_PARALLEL_RUN();

public:
  PushToMySQL() = default;
  virtual ~PushToMySQL() = default;

protected:
  PushToMySQL(const PushToMySQL &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

private:
  int chatter_ = 0;
  InterpretTelemetry *HubHKinterpreter_ = nullptr;
  std::string HubHKInterpreterName_ = "InterpretTelemetry";
  std::string HubHKtableName_ = "HubHK";

  MySQLManager *mysqlManager_ = nullptr;

  MySQLFieldSink mysqlFieldSink_;
};
} // namespace pgrams
} // namespace gramsballoon
#endif //GRAMSBalloon_PushToMySQL_hh