#include "CommandSender.hh"

#include <cstdint>
#include <iostream>
#include <vector>

namespace gramsballoon::pgrams {

int CommandSender::open_mosquitto() {
  const int status = mosquittoIo_->Connect();
  if (status != 0) {
    std::cerr << "Failed to connect to mosquitto" << std::endl;
    return status;
  }
  for (int i = 0; i < 1; i++) {
    mosquittoIo_->loop();
  }
  return 0;
}

int CommandSender::send(const std::string &topic, const std::vector<uint8_t> &byte_array) {
  if (mosquittoIo_ == nullptr) {
    std::cerr << "Mosquitto not open" << std::endl;
    return -1;
  }
  const int status = mosquittoIo_->Publish(byte_array, topic);
  if (status != 0) {
    std::cerr << "Failed to publish message" << std::endl;
    return status;
  }
  for (int i = 0; i < 1; i++) {
    mosquittoIo_->loop();
  }
  return 0;
}

} // namespace gramsballoon::pgrams
