/**
 * Module for distributing command to other subsystems.
 *
 * @author Shota Arai
 * @date 2025-02-** | First design
 * @date 2025-03-24 | Delete socket communication feature and make based on SocketCommunicationManager
 *
 */
#ifndef GB_DistributeCommand_hh
#define GB_DistributeCommand_hh 1
#include "MosquittoManager.hh"
#include "SendTelemetry.hh"
#include "SocketCommunicationManager.hh"
#include "anlnext/BasicModule.hh"
#include "sys/socket.h"
#include <arpa/inet.h>
#include <map>
#include <string>
namespace gramsballoon::pgrams {
class SendTelemetry;
template <typename T>
class MosquittoManager;

class DistributeCommand: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(DistributeCommand, 2.0);
  ENABLE_PARALLEL_RUN();

public:
  DistributeCommand() = default;
  virtual ~DistributeCommand() = default;

protected:
  DistributeCommand(const DistributeCommand &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;
  std::shared_ptr<mqtt::mosquitto_message<std::vector<uint8_t>>> getAndPopPayload() {
    if (payloads_.empty()) {
      return nullptr;
    }
    auto payload = payloads_.front();
    payloads_.pop_front();
    return payload;
  }

private:
  MosquittoManager<std::vector<uint8_t>> *mosquittoManager_ = nullptr;
  SendTelemetry *sendTelemetry_ = nullptr;
  std::string socketCommunicationManagerName_ = "SocketCommunicationManager";
  std::string topic_ = "command";
  std::deque<std::shared_ptr<mqtt::mosquitto_message<std::vector<uint8_t>>>> payloads_;
  int chatter_ = 0;
  bool failed_ = false;
};
} // namespace gramsballoon::pgrams
#endif // DistributeCommand_hh