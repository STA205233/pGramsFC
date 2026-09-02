#ifndef GRAMSBalloon_PushToMySQL_hh
#define GRAMSBalloon_PushToMySQL_hh 1
#include "MySQLFieldSink.hh"
#include "MySQLManager.hh"
#include "VDBDataStore.hh"
#include "anlnext/BasicModule.hh"
#include <string>
#include <vector>

namespace gramsballoon {
namespace pgrams {
class MySQLFieldSink;
class InterpretTelemetry;
/**
 * @brief Module for pushing telemetry data to MySQL database.
 * @author Shota Arai
 * @date 2025-**-** | First design
 * @date 2025-12-14 | Shota Arai | Refactoring (v2.0)
 * @date 2026-02-04 | Shota Arai | Use MySQLManager (v3.0)
 * @date 2026-07-09 | Shota Arai | Use of VDBDataStore (v3.1)
 */
class PushToMySQL: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(PushToMySQL, 3.1);
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
  const VDBDataStore *dataStore_ = nullptr;
  std::string dataStoreName_ = "InterpretTelemetry";
  std::string tableName_ = "HubHK";

  MySQLManager *mysqlManager_ = nullptr;

  MySQLFieldSink mysqlFieldSink_;
};
} // namespace pgrams
} // namespace gramsballoon
#endif //GRAMSBalloon_PushToMySQL_hh