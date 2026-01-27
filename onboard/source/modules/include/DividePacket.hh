#ifndef DivideCommand_hh
#define DivideCommand_hh 1
#include "CommunicationFormat.hh"
#include "CommunicationSaver.hh"
#include "IridiumPacketPool.hh"
#include "ReceiveStatusFromDAQComputer.hh"
#include "anlnext/BasicModule.hh"
#include <queue>
#include <tuple>
#include <vector>
namespace gramsballoon::pgrams {

class ReceiveStatusFromDAQComputer;
class IridiumPacketPool;
class BaseTelemetryDefinition;
template <typename T>
class CommunicationSaver;
class DividePacket: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(DividePacket, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  DividePacket();
  virtual ~DividePacket() = default;

protected:
  DividePacket(const DividePacket &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;
  bool IsEmptyIridium() const {
    return singleton_self()->iridiumPacketQueue_->empty();
  }
  const std::shared_ptr<BaseTelemetryDefinition> GetLastPacketIridium() const {
    return singleton_self()->iridiumPacketQueue_->front();
  }
  void PopPacketIridium() {
    if (!singleton_self()->iridiumPacketQueue_->empty())
      singleton_self()->iridiumPacketQueue_->pop();
  }
  bool IsEmptyStarlink() const {
    return singleton_self()->starlinkPacketQueue_.empty();
  }
  const std::shared_ptr<BaseTelemetryDefinition> GetLastPacketStarlink() const {
    return singleton_self()->starlinkPacketQueue_.front();
  }
  void PopPacketStarlink() {
    if (!singleton_self()->starlinkPacketQueue_.empty())
      singleton_self()->starlinkPacketQueue_.pop();
  }
  void PopPacket(bool isStarlink) {
    if (isStarlink) {
      singleton_self()->PopPacketStarlink();
    }
    else {
      singleton_self()->PopPacketIridium();
    }
  }
  bool IsEmpty(bool isStarlink) const {
    if (isStarlink) {
      return singleton_self()->IsEmptyStarlink();
    }
    else {
      return singleton_self()->IsEmptyIridium();
    }
  }
  bool IsPacketPushed() const {
    return !singleton_self()->lastPushedPackets_.empty();
  }
  const auto &GetLastPushedPackets() const {
    return singleton_self()->lastPushedPackets_;
  }
  const std::shared_ptr<BaseTelemetryDefinition> GetLastPacket(bool isStarlink) const {
    if (isStarlink) {
      return singleton_self()->GetLastPacketStarlink();
    }
    else {
      return singleton_self()->GetLastPacketIridium();
    }
  }
  Subsystem GetSubsystem() const {
    return singleton_self()->subsystem_;
  }

private:
  void PushCurrentVector();
  ReceiveStatusFromDAQComputer *receiveStatusFromDAQComputer_ = nullptr;
  std::string receiveStatusFromDAQComputerName_ = "ReceiveStatusFromDAQComputer";
  std::vector<int> starlinkCode_;
  size_t maxPacketSize_ = 10000; // in bytes
  std::shared_ptr<IridiumPacketPool> iridiumPacketQueue_;
  uint16_t overwrittenPacketCode_ = UINT16_MAX;
  std::queue<std::shared_ptr<BaseTelemetryDefinition>> starlinkPacketQueue_;
  std::vector<std::shared_ptr<BaseTelemetryDefinition>> lastPushedPackets_;
  std::vector<uint8_t> currentPacket_;
  int chatter_ = 0;
  size_t lastPacketSize_ = 0;
  uint32_t index_ = 0;
  uint16_t currentCode_ = 0;
  bool inError_ = false;
  static constexpr int MAX_BYTES = 1024;
  std::shared_ptr<CommunicationSaver<std::vector<uint8_t>>> statusSaver_ = nullptr;
  bool saveStatus_ = false;
  std::string binaryFilenameBase_ = "status";
  int numStatusPerFile_ = 100;
  Subsystem subsystem_ = Subsystem::UNKNOWN;
};
} // namespace gramsballoon::pgrams
#endif // DivideCommand_hh