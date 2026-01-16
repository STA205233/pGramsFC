/**
 * Module for Sending telemetry.
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-25
 * @date 2025-**-** | v2.0
 * @date 2025-09-20 | Json-wrapped telemetry. (v3.0)
 */

#ifndef SendTelemetry_H
#define SendTelemetry_H 1

#include "BaseTelemetryDefinition.hh"
#include "CommunicationSaver.hh"
#include "ErrorManager.hh"
#include "GetComputerStatus.hh"
#include "GetMHADCData.hh"
#include "HubHKTelemetry.hh"
#include "MHADCMapping.hh"
#include "MeasureOrientationByMHADC.hh"
#include "MosquittoManager.hh"
#include "ReceiveCommand.hh"
#include "RunIDManager.hh"
#include <anlnext/BasicModule.hh>
#include <thread>

namespace gramsballoon {

class MeasureTemperatureWithRTDSensor;
class ReceiveCommand;
class RunIDManager;
} // namespace gramsballoon
namespace gramsballoon::pgrams {
class GetMHADCData;
class GetPressure;
template <typename T>
class MosquittoManager;
class DistributeCommand;
class BaseTelemetryDefinition;
class HubHKtelemetry;
class ErrorManager;
template <typename T>
class CommunicationSaver;
class MHADCMapping;
class GetComputerStatus;

class SendTelemetry: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(SendTelemetry, 3.1);
  ENABLE_PARALLEL_RUN();

public:
  SendTelemetry();
  virtual ~SendTelemetry();

protected:
  SendTelemetry(const SendTelemetry &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  void setTelemetryType(int v) { singleton_self()->telemetryType_ = v; }
  int TelemetryType() { return singleton_self()->telemetryType_; }

  void setLastComIndex(Subsystem subsystem, uint32_t v);
  void setLastComCode(Subsystem subsystem, uint16_t v);

  ErrorManager *getErrorManager() { return (singleton_self()->errorManager_).get(); }

private:
  void setHKTelemetry();
  void getHKModules();
  std::shared_ptr<HubHKTelemetry> telemdef_ = nullptr;
  int telemetryType_ = 1;
  std::shared_ptr<ErrorManager> errorManager_ = nullptr;
  bool saveTelemetry_ = true;
  std::shared_ptr<CommunicationSaver<std::string>> telemetrySaver_ = nullptr;
  std::shared_ptr<MHADCMapping> mhadcMapping_ = nullptr;
  uint32_t telemIndex_ = 0;
  std::string binaryFilenameBase_ = "";
  int numTelemPerFile_ = 1000;
  int minimumSendTime_ = 1000; // in milliseconds
  std::chrono::steady_clock::time_point lastSendTime_;
  int chatter_ = 0;
  std::string telemetryStr_ = "";

  // access to other classes
  ReceiveCommand *receiveCommand_ = nullptr;
  RunIDManager *runIDManager_ = nullptr;
  pgrams::MosquittoManager<std::string> *mosquittoManager_ = nullptr;

  pgrams::MosquittoIO<std::string> *mosq_ = nullptr;
  std::string pubTopic_ = "telemetry";
  std::string starlinkTopic_ = "StarlinkTelemetry";
  int qos_ = 0;

  // HK data modules
  const GetMHADCData *getMhadcData_ = nullptr;
#ifdef USE_SYSTEM_MODULES
  const GetComputerStatus *getComputerStatus_ = nullptr;
#endif
};

} /* namespace gramsballoon::pgrams */

#endif /* SendTelemetry_H */
