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

class PushTBTelemetryToMySQL final: anlnext::BasicModule {
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
  void initializeMysqlField(MySQLFieldSink *field_sink, mysql::MySQLIO *mysql_io, const std::string &table_name);
};
} // namespace gramsballoon::pgrams
#endif //GB_PushTBTelemetryToMySQL_hh