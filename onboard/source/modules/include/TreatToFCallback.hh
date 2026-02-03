#ifndef GRAMSBalloon_TreatToFCallback_hh
#define GRAMSBalloon_TreatToFCallback_hh 1
#include "InterpretTelemetry.hh"
#include "MySQLFieldSink.hh"
#include "MySQLIO.hh"
#include "anlnext/BasicModule.hh"
#include <cstdint>
namespace gramsballoon::pgrams {
class InterpretTelemetry;

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

  std::string password_ = "test";
  std::string database_ = "pgrams";
  std::string user_ = "test";
  int port_ = 33060;
  std::string host_ = "localhost";
  bool checkExist_ = true;
  mysql::MySQLIO mysqlIO_;
  MySQLFieldSink mysqlFieldSink_;
  const InterpretTelemetry *interpretTelemetry_ = nullptr;
  std::string interpretTelemetryName_ = "InterpretTelemetry";
  uint8_t statusCode_;
  TOF_STATUS status_;
  uint16_t command_;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_TreatToFCallback_hh