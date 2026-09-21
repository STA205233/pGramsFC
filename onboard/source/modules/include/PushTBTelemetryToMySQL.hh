#ifndef GB_PushTBTelemetryToMySQL_hh
#define GB_PushTBTelemetryToMySQL_hh 1
#include "anlnext/BasicModule.hh"
#include <memory>
#include <string>

namespace gramsballoon::pgrams {
class InterpretTelemetry;
class MySQLFieldSink;
class MySQLManager;
namespace mysql {
class MySQLIO;
}

/**
 * @brief Module to push the ToF bias telemetry interpreted by InterpretTelemetry into the MySQL database
 * @author Shota Arai
 * @date 2026-09-04 | Shota Arai | First version
 * @ingroup ground_module
 */
class PushTBTelemetryToMySQL final: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(PushTBTelemetryToMySQL, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  PushTBTelemetryToMySQL() = default;
  virtual ~PushTBTelemetryToMySQL() = default;

protected:
  PushTBTelemetryToMySQL(const PushTBTelemetryToMySQL &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

private:
  int chatter_ = 0;
  const InterpretTelemetry *interpretTelemetry_ = nullptr;
  std::string interpretTelemetryName_ = "InterpretTelemetry";
  std::string tableNameForFullOutput_ = "TofBias_full";
  std::string tableNameForSummaryOutput_ = "TofBias_summary";

  MySQLManager *mysqlManager_ = nullptr;

  std::shared_ptr<MySQLFieldSink> mysqlFieldSinkForFull_ = nullptr;
  std::shared_ptr<MySQLFieldSink> mysqlFieldSinkForSummary_ = nullptr;

private:
  void initializeMysqlField(mysql::MySQLIO *mysql_io);
};
} // namespace gramsballoon::pgrams
#endif //GB_PushTBTelemetryToMySQL_hh