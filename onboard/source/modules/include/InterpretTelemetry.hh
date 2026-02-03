#ifndef InterpretTelemetry_H
#define InterpretTelemetry_H 1

#include "BaseTelemetryDefinition.hh"
#include "ErrorManager.hh"
#include "ReceiveTelemetry.hh"
#include <anlnext/BasicModule.hh>
#include <chrono>
#include <thread>
#ifdef USE_ROOT
#endif // USE_ROOT
#ifdef USE_HSQUICKLOOK
#include "PushToMongoDB.hh"
#endif // USE_HSQUICKLOOK
namespace gramsballoon::pgrams {
class ReceiveTelemetry;
class BaseTelemetryDefinition;
#ifdef USE_HSQUICKLOOK
class PushToMongoDB;
#endif // USE_HSQUICKLOOK

/**
 * Module for interpretation of telemetry
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-**-**
 * @date 2025-09-20 Shota Arai| Comparatible to different type of telemetry. (v2.0)
 * @date 2025-11-17 Shota Arai| Refactoring
 * @date 2025-12-14 Shota Arai| Added DB serialization functions
 */
class InterpretTelemetry: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(InterpretTelemetry, 2.0);
  ENABLE_PARALLEL_RUN();

public:
  InterpretTelemetry();
  virtual ~InterpretTelemetry() = default;

protected:
  InterpretTelemetry(const InterpretTelemetry &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  std::shared_ptr<const ErrorManager> getErrorManager() const { return singleton_self()->errorManager_; }
  int CurrentTelemetryType() const { return singleton_self()->currentTelemetryType_; }
  std::string_view TelemetryType() {
    return singleton_self()->telemetryTypeStr_;
  }
  void pushToDBSink(DBFieldSink *sink) const {
    if (singleton_self()->telemetry_) {
      singleton_self()->telemetry_->serialize(sink);
    }
  }
  void initializeDBTableInSink(DBFieldSink *sink, const std::string &table_name) const {
    if (singleton_self()->telemetry_) {
      singleton_self()->telemetry_->initializeDBTable(sink, table_name);
    }
  }
  const BaseTelemetryDefinition *getTelemetry() const {
    return telemetry_.get();
  }

private:
  void updateRunIDFile();
  bool interpret(const std::string &telemetryStr);
  std::shared_ptr<BaseTelemetryDefinition> telemetry_ = nullptr;
  pgrams::ReceiveTelemetry *receiver_ = nullptr;
  std::shared_ptr<ErrorManager> errorManager_ = nullptr;
  std::string telemetryTypeStr_ = "HK";
#ifdef USE_HSQUICKLOOK
  PushToMongoDB *pusher_ = nullptr;
#endif // USE_HSQUICKLOOK
  std::shared_ptr<CommunicationSaver<std::string>> telemetrySaver_ = nullptr;
  int currentTelemetryType_ = 0; // 1: normal, 2: tof callback
  std::map<int, std::pair<int, int>> fileIDmp_;
  bool saveTelemetry_ = true;
  std::string binaryFilenameBase_ = "";
  int numTelemPerFile_ = 100;
  int chatter_ = 0;
  std::string runIDFilename_;
  int currentRunID_ = -1;
  std::string receiverModuleName_;
};

} // namespace gramsballoon::pgrams
#endif // InterpretTelemetry_H
