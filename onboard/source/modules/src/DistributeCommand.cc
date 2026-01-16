#include "DistributeCommand.hh"
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <string>
using namespace anlnext;

namespace gramsballoon::pgrams {
ANLStatus DistributeCommand::mod_define() {
  define_parameter("topic", &mod_class::topic_);
  set_parameter_description("Topic to subscribe");
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus DistributeCommand::mod_initialize() {
  if (exist_module("ComMosquittoManager")) {
    get_module_NC("ComMosquittoManager", &mosquittoManager_);
  }
  else {
    std::cerr << "ComMosquittoManager module is not found." << std::endl;
    return AS_ERROR;
  }
  auto mosq = mosquittoManager_->getMosquittoIO();
  if (!mosq) {
    std::cerr << "MosquittoIO in the MosquittoManager is nullptr." << std::endl;
    return AS_ERROR;
  }

  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  else {
    std::cerr << "SendTelemetry module is not found." << std::endl;
  }

  mosq->Subscribe(topic_.c_str(), 0);
  failed_ = false;
  if (chatter_ > 0) {
    std::cout << "Connected to " << topic_ << std::endl;
  }
  return AS_OK;
}

ANLStatus DistributeCommand::mod_analyze() {
  if (!mosquittoManager_) {
    std::cerr << "DistributeCommand::mod_analyze: MosquittoManager is nullptr." << std::endl;
    return AS_OK;
  }
  auto mosq = mosquittoManager_->getMosquittoIO();
  if (!mosq) {
    std::cerr << "MosquittoIO in the MosquittoManager is nullptr." << std::endl;
    return AS_OK;
  }
  auto commands = mosq->getPayload();
  if (commands.empty()) {
    return AS_OK;
  }
  const auto command = commands.front();
  if (topic_ == command->topic) {
    if (chatter_ > 0) {
      std::cout << "Received command for " << topic_ << std::endl;
    }
    payloads_.push_back(command);
    mosq->popPayloadFront();
  }
  return AS_OK;
}
ANLStatus DistributeCommand::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams