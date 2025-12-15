/**
 * Module for managing the socket communication.
 *
 * @author Shota Arai
 * @date 2025-03-24 | first draft
 *
 */
#ifndef GRAMSBalloon_SocketCommunicationManager_hh
#define GRAMSBalloon_SocketCommunicationManager_hh 1
#include "IoContextManager.hh"
#include "SendTelemetry.hh"
#include "SocketCommunicationServer.hh"
#include "anlnext/BasicModule.hh"
#include <optional>
namespace gramsballoon::pgrams {
class SendTelemetry;
enum class AcknowledgementType {
  SIZE = 0,
  RAW = 1,
  NONE = 2
};
class IoContextManager;

class SocketCommunicationManager: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(SocketCommunicationManager, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  SocketCommunicationManager() = default;
  virtual ~SocketCommunicationManager() = default;

protected:
  SocketCommunicationManager(const SocketCommunicationManager &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_pre_initialize() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;
  SocketCommunication *getSocketCommunication() {
    return singleton_self()->socketCommunication_.get();
  }
  int sendAndWaitForAck(const uint8_t *buf, size_t n, const uint8_t *ack, size_t ack_n);
    int send(const uint8_t *buf, size_t n) {
    return singleton_self()->socketCommunication_->send(buf, n);
  }
  inline int sendAndWaitForAck(const std::vector<uint8_t> &data, const std::vector<uint8_t> &ack) {
    return sendAndWaitForAck(data.data(), data.size(), ack.data(), ack.size());
  }
  int receive(std::vector<uint8_t> &data);

private:
  std::string ip_;
  int port_;
  int chatter_ = 0;
  std::shared_ptr<SocketCommunication> socketCommunication_ = nullptr;
  SendTelemetry *sendTelemetry_ = nullptr;
  IoContextManager *ioContextManager_ = nullptr;
  std::shared_ptr<boost::asio::io_context> ioContext_ = nullptr;
  int timeout_ = -1;
  std::optional<timeval> timeoutTV_;
  bool handleSigpipe_ = false;
  int subsystemInt_ = 0;
  Subsystem subsystem_ = Subsystem::UNKNOWN;
  std::shared_ptr<std::thread> thread_ = nullptr;
  std::vector<uint8_t> ackBuffer_;
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_SocketCommunicationManager_hh