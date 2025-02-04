#include "CommandSender.hh"

#include <cstdint>
#include <iostream>
#include <vector>

namespace gramsballoon::pgrams {

int CommandSender::open_mosquitto() {
  mosquittoIo_ = std::make_shared<MosquittoIO<std::vector<uint8_t>>>(id_, host_, port_, keepAlive_);
  const int status = mosquittoIo_->Connect();
  if (status != 0) {
    std::cerr << "Failed to connect to mosquitto" << std::endl;
    return status;
  }
  return 0;
}

int CommandSender::send(const std::string &topic, const std::vector<uint8_t> &byte_array) {
  if (mosquittoIo_ == nullptr) {
    std::cerr << "Mosquitto not open" << std::endl;
    return -1;
  }
  return mosquittoIo_->Publish(byte_array, topic);
}

} // namespace gramsballoon::pgrams
