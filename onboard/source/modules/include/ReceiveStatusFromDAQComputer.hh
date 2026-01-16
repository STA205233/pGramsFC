/**
 * Module for receiving from DAQ computers.
 *
 * @author Shota Arai
 * @date 2025-03-24 | First design
 *
 */
#ifndef GB_ReceiveStatusFromDAQComputer_hh
#define GB_ReceiveStatusFromDAQComputer_hh 1
#include "CommunicationFormat.hh"
#include "SendTelemetry.hh"
#include "SocketCommunicationManager.hh"
#include "anlnext/BasicModule.hh"
#include <chrono>
#include <memory>
#include <queue>
#include <string>
namespace gramsballoon::pgrams {
class SendTelemetry;
class SocketCommunicationManager;
class ReceiveStatusFromDAQComputer: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(ReceiveStatusFromDAQComputer, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  ReceiveStatusFromDAQComputer() = default;
  virtual ~ReceiveStatusFromDAQComputer() = default;

protected:
  ReceiveStatusFromDAQComputer(const ReceiveStatusFromDAQComputer &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;
  uint8_t PopAndGetOneByte() {
    const uint8_t byte = singleton_self()->buffer_.front();
    singleton_self()->buffer_.pop_front();
    return byte;
  }
  size_t GetBufferSize() {
    return singleton_self()->buffer_.size();
  }
  Subsystem getSubsystem() const {
    return singleton_self()->subsystem_;
  }

private:
  SocketCommunicationManager *socketCommunicationManager_ = nullptr;
  SendTelemetry *sendTelemetry_ = nullptr;
  std::string socketCommunicationManagerName_ = "SocketCommunicationManager";
  int chatter_ = 0;
  std::deque<uint8_t> buffer_;
  std::vector<uint8_t> bufTmp_;
  int deadCommunicationTime_ = 1000; // in milliseconds
  std::chrono::milliseconds deadCommunicationTimeChrono_;
  std::chrono::time_point<std::chrono::steady_clock> lastReceivedTime_;
  static constexpr int MAX_BYTES = 1024;
  bool saveStatus_ = false;
  std::shared_ptr<CommunicationSaver<std::vector<uint8_t>>> statusSaver_ = nullptr;
  std::string binaryFilenameBase_ = "";
  int numStatusPerFile_ = 100;
  int subsystemInt_ = 1;
  Subsystem subsystem_ = Subsystem::UNKNOWN;
};
} // namespace gramsballoon::pgrams
#endif // GB_ReceiveStatusFromDAQComputer_hh