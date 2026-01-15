#include "DistributeCommand.hh"
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
using namespace anlnext;

void SigPipeHandler(int) {
  std::cout << "Caught SIGPIPE!" << std::endl;
}
namespace gramsballoon::pgrams {
DistributeCommand::DistributeCommand() {
  //subSystems_ = {
  //    {"TPC", SubSystem(-1, 50000, "localhost", "TPC")},
  //    {"SiPM", SubSystem(-1, 50001, "localhost", "SiPM")},
  //    {"DAQ", SubSystem(-1, 50002, "localhost", "DAQ")},
  //    {"TOF", SubSystem(-1, 50003, "localhost", "TOF")}};
}
ANLStatus DistributeCommand::mod_define() {
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus DistributeCommand::mod_initialize() {
  if (exist_module("MosquittoManager")) {
    get_module_NC("MosquittoManager", &mosquittoManager_);
  }
  else {
    std::cerr << "MosquittoManager module is not found." << std::endl;
    return AS_ERROR;
  }
  mosq_ = mosquittoManager_->getMosquittoIO();
  if (!mosq_) {
    std::cerr << "MosquittoIO in the MosquittoManager is nullptr." << std::endl;
    return AS_ERROR;
  }

  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  else {
    std::cerr << "SendTelemetry module is not found." << std::endl;
    return AS_ERROR;
  }

  /// Set SIGPIPE handler in case of broken pipe
  struct sigaction sa;
  sa.sa_handler = SigPipeHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGPIPE, &sa, NULL);

  ///// Initialize sockets
  for (auto &subSystem: subSystems_) {
    mosq_->subscribe(NULL, subSystem.second.topic.c_str(), 0);
    subSystem.second.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (subSystem.second.socket < 0) {
      std::cerr << "Error in DistributeCommand::mod_initialize: Socket creation failed." << std::endl;
      return AS_ERROR;
    }

    failed_ = false;
    for (int i = 0; i < numTrial_; i++) {
      if (connect(subSystem.second.socket, (sockaddr *)&subSystem.second.serverAddress, sizeof(subSystem.second.serverAddress)) < 0) {
        std::cerr << "Error in DistributeCommand::mod_initialize: Connection failed." << std::endl;
        failed_ = true;
        continue;
      }
      failed_ = false;
      break;
    }
    if (failed_) {
      std::cerr << "Error in DistributeCommand::mod_initialize: Connection failed, despite " << numTrial_ << " times trials." << std::endl;
      if (sendTelemetry_) {
        sendTelemetry_->getErrorManager()->setError(ErrorType::RECEIVE_COMMAND_SERIAL_COMMUNICATION_ERROR); // TODO: This flag is to be defined
      }
    }
    else {
      if (chatter_ > 0) {
        std::cout << "Connected to " << subSystem.first << std::endl;
      }
      if (chatter_ > 1) {
        std::cout << "IP: " << subSystem.second.ip << std::endl;
        std::cout << "Port: " << subSystem.second.port << std::endl;
      }
      if (chatter_ > 2) {
        std::cout << "Socket: " << subSystem.second.socket << std::endl;
      }
    }
  }
  return AS_OK;
}

ANLStatus DistributeCommand::mod_analyze() {
  if (!mosq_) {
    std::cerr << "MosquittoIO in the MosquittoManager is nullptr." << std::endl;
    return AS_OK;
  }
  auto commands = mosq_->getPayload();
  if (commands.empty()) {
    return AS_OK;
  }
  const auto command = commands.front();
  const auto &command_payload = command->payload;
  for (const auto &subSystem: subSystems_) {
    const auto &topic = subSystem.second.topic;
    if (topic == command->topic) {
      if (chatter_ > 0) {
        std::cout << "Received command for " << subSystem.first << std::endl;
      }
      if (subSystem.second.socket == 0) {
        std::cerr << "Socket is not opened." << std::endl;
        mosq_->popPayloadFront();
        return AS_OK;
      }
      for (int i = 0; i < numTrial_; i++) {
        const auto send_result = send(subSystem.second.socket, command_payload.data(), command_payload.size(), 0); // TODO: this depends on telemetry definition.
        if (send_result == -1) {
          std::cerr << "Error in DistributeCommand::mod_analyze: " << "Trial " << i << " Sending data failed." << std::endl;
          failed_ = true;
          continue;
        }
        else if (chatter_ > 0) {
          std::cout << "Sent data to " << subSystem.first << std::endl;
          std::cout << "Payload size: " << send_result << std::endl;
        }
        failed_ = false;
        break;
      }
      if (failed_) {
        std::cerr << "Error in DistributeCommand::mod_analyze: Sending data failed, despite " << numTrial_ << " times trials." << std::endl;
      }
      else {
        if (chatter_ > 1) {
          std::cout << "Payload:" << std::endl;
          for (const auto &byte: command_payload) {
            std::cout << static_cast<int>(byte) << " ";
          }
          std::cout << std::endl;
        }
      }
      mosq_->popPayloadFront();
      break;
    }
  }
  return AS_OK;
}
ANLStatus DistributeCommand::mod_finalize() {
  for (const auto &subSystem: subSystems_) {
    if (subSystem.second.socket != 0) {
      close(subSystem.second.socket);
    }
  }
  return AS_OK;
}
} // namespace gramsballoon::pgrams