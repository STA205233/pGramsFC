#ifndef GRAMSBalloon_SendCommandToDAQComputer_hh
#define GRAMSBalloon_SendCommandToDAQComputer_hh 1
#include "CommunicationFormat.hh"
#include "DistributeCommand.hh"
#include "SendTelemetry.hh"
#include "SocketCommunicationManager.hh"
#include "anlnext/BasicModule.hh"
#include <chrono>
namespace gramsballoon::pgrams {
class SendTelemetry;
class SocketCommunicationManager;
class DistributeCommand;

/**
 * Module for sending command to DAQ computer via SocketCommunicationManager.
 *
 * @author Shota Arai
 * @date 2025-**-** | First design
 * @date 2025-12-14 | Shota Arai | Added emergency DAQ shutdown command
 * @date 2026-02-04 | Shota Arai | Added command reject count (v1.2)
 */
class SendCommandToDAQComputer: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(SendCommandToDAQComputer, 1.2);
  ENABLE_PARALLEL_RUN();

public:
  SendCommandToDAQComputer() = default;
  virtual ~SendCommandToDAQComputer() = default;

protected:
  SendCommandToDAQComputer(const SendCommandToDAQComputer &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

  void setEmergencyDaqShutdown(const bool val) {
    singleton_self()->emergencyDaqShutdownReceived_ = val;
  }
  bool EmergencyDaqShutdown() const {
    return singleton_self()->emergencyDaqShutdownReceived_;
  }
  uint16_t CommandRejectCount() const {
    return singleton_self()->commandRejectCount_;
  }

private:
  SocketCommunicationManager *socketCommunicationManager_ = nullptr;
  std::string socketCommunicationManagerName_ = "SocketCommunicationManager";
  int chatter_ = 0;
  DistributeCommand *distributeCommand_ = nullptr;
  std::string distributeCommandName_ = "DistributeCommand";
  SendTelemetry *sendTelemetry_ = nullptr;
  int subsystemInt_ = 0;
  Subsystem subsystem_ = Subsystem::UNKNOWN;
  std::shared_ptr<std::chrono::time_point<std::chrono::high_resolution_clock>> lastTime_ = nullptr;
  uint32_t commandIndex_ = 0;
  int durationBetweenHeartbeat_ = 800; // ms
  std::shared_ptr<std::chrono::milliseconds> durationBetweenHeartbeatChrono_ = nullptr;
  std::shared_ptr<CommunicationFormat> heartbeat_ = nullptr;
  std::shared_ptr<CommunicationFormat> heartbeatAck_ = nullptr;
  std::shared_ptr<CommunicationFormat> currentCommand_ = nullptr;
  std::shared_ptr<CommunicationFormat> commandAck_ = nullptr;

  uint16_t commandRejectCount_ = 0;

  bool emergencyDaqShutdownReceived_ = false;
  bool makeDAQEmergencyShutdownCommand();
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_SendCommandToDAQComputer_hh