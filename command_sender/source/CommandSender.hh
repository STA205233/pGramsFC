#ifndef GRAMSBALOON_COMMANDSENDER_H
#define GRAMSBALOON_COMMANDSENDER_H 1

#include "MosquittoIO.hh"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace gramsballoon::pgrams {

class CommandSender {
public:
  CommandSender() = default;
  CommandSender(const std::string &host, const int port) : host_(host), port_(port) {}
  ~CommandSender() = default;

  int open_mosquitto();
  int send(const std::string &topic, const std::vector<uint8_t> &byte_array);
  int close_mosquitto() {
    if (mosquittoIo_ == nullptr) {
      return 0;
    }
    mosquittoIo_->loop_stop();
    return mosquittoIo_->Disconnect();
  }
  int authentication(const std::string &username, const std::string &password) {
    return mosquittoIo_->username_pw_set(username.c_str(), password.c_str());
  }

private:
  std::shared_ptr<MosquittoIO<std::vector<uint8_t>>> mosquittoIo_ = nullptr;
  const std::string id_ = "CommandSender";
  const std::string host_ = "localhost";
  const int port_ = 1883;
  const int keepAlive_ = 60;
};

} // namespace gramsballoon::pgrams

#endif /* GRAMSBALOON_COMMANDSENDER_H */
