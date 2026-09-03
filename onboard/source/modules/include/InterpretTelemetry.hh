#ifndef InterpretTelemetry_H
#define InterpretTelemetry_H 1

#include "BaseTelemetryDefinition.hh"
#include "ErrorManager.hh"
#include "ReceiveTelemetry.hh"
#include "VDBDataStore.hh"
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
template <typename T>
class CommunicationSaver;
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
 * @date 2026-06-16 Shota Arai | Added some functions (v2.1)
 * @date 2026-07-09 Shota Arai | Inherited from VDBDataStore (v2.2)
 */
class InterpretTelemetry: public anlnext::BasicModule, public VDBDataStore {
  DEFINE_ANL_MODULE(InterpretTelemetry, 2.2);
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
  uint16_t CurrentTelemetryType() const { return singleton_self()->currentTelemetryType_; }
  std::string_view TelemetryType() {
    return singleton_self()->telemetryTypeStr_;
  }

  void pushToDBSink(DBFieldSink *sink) const override {
    if (singleton_self()->telemetry_) {
      singleton_self()->telemetry_->serialize(sink);
    }
  }
  void initializeDBTableInSink(DBFieldSink *sink, const std::string &table_name) const override {
    if (singleton_self()->telemetry_) {
      singleton_self()->telemetry_->initializeDBTable(sink, table_name);
    }
  }
  bool hasData() const override {
    return singleton_self()->telemetry_ && (singleton_self()->CurrentTelemetryType() != 0);
  }

  const BaseTelemetryDefinition *getTelemetry() const {
    return singleton_self()->telemetry_.get();
  }
  std::shared_ptr<BaseTelemetryDefinition> getTelemetryShared() const {
    return singleton_self()->telemetry_;
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
  uint16_t currentTelemetryType_ = 0; // following the CommunicationCodes.hh
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
