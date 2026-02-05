#include "SendCommandToDAQComputer.hh"
#include "CommunicationCodes.hh"
using namespace pgrams::communication;
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus SendCommandToDAQComputer::mod_define() {
  define_parameter("SocketCommunicationManager_name", &mod_class::socketCommunicationManagerName_);
  set_parameter_description("Name of SocketCommunicationManager");
  define_parameter("DistributeCommand_name", &mod_class::distributeCommandName_);
  set_parameter_description("Name of DistributeCommand");
  define_parameter("duration_between_heartbeat", &mod_class::durationBetweenHeartbeat_);
  set_parameter_description("Duration between heartbeat in ms");
  set_parameter_unit(1.0, "ms");
  define_parameter("subsystem", &mod_class::subsystemInt_);
  set_parameter_description("Subsystem enum value");
  define_parameter("chatter", &mod_class::chatter_);
  set_parameter_description("Chatter level");
  return AS_OK;
}
ANLStatus SendCommandToDAQComputer::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize SendTelemetry module is not found." << std::endl;
  }
  if (exist_module(socketCommunicationManagerName_)) {
    get_module_NC(socketCommunicationManagerName_, &socketCommunicationManager_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize SocketCommunicationManager module is not found." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
  }
  if (exist_module(distributeCommandName_)) {
    get_module_NC(distributeCommandName_, &distributeCommand_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize DistributeCommand module is not found." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
  }
  subsystem_ = magic_enum::enum_cast<Subsystem>(subsystemInt_).value_or(Subsystem::UNKNOWN);
  durationBetweenHeartbeatChrono_ = std::make_shared<std::chrono::milliseconds>(durationBetweenHeartbeat_);
  heartbeat_ = std::make_shared<CommunicationFormat>();
  if (heartbeat_) {
    heartbeat_->setCode(castCommandCode(CommunicationCodes::COM_HeartBeat));
    heartbeat_->setArgc(1); // index of the heartbeat
    heartbeat_->setArguments(0, -1);
    heartbeat_->update();
  }
  else {
    std::cerr << module_id() << "::mod_initialize Failed to create CommunicationFormat." << std::endl;
    return AS_ERROR;
  }
  heartbeatAck_ = std::make_shared<CommunicationFormat>();
  if (heartbeatAck_) {
    heartbeatAck_->setCode(castCommandCode(CommunicationCodes::COM_HeartBeat));
    heartbeatAck_->setArgc(1); // index of the heartbeat
    heartbeatAck_->setArguments(0, heartbeat_->Command().size());
    heartbeatAck_->update();
  }

  //failed_ = false;
  return AS_OK;
}
ANLStatus SendCommandToDAQComputer::mod_analyze() {
  if (!distributeCommand_) return AS_OK;
  if (EmergencyDaqShutdown()) {
    if (chatter_ > 0) {
      std::cout << module_id() << ": Emergency DAQ shutdown received. Skipping sending commands to DAQ computer." << std::endl;
    }
    const bool exists_command = makeDAQEmergencyShutdownCommand();
    if (exists_command) {
      performDAQEmergencyShutdown();
    }
    return AS_OK;
  }

  auto m_sptr = distributeCommand_->getAndPopPayload();
  if (!socketCommunicationManager_->isConnected()) {
    if (chatter_ > 1) {
      std::cout << module_id() << ": SocketCommunication is not connected." << std::endl;
    }
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqComErrorType(subsystem_, true));
    }
    //failed_ = true;
    if (m_sptr) {
      m_sptr.reset();
      return AS_OK;
    }
  }
  if (!m_sptr) { // No command to send, check heartbeat should be sent.
    sendHeartbeatIfNeeded();
    return AS_OK;
  }

  if (!currentCommand_) {
    currentCommand_ = std::make_shared<CommunicationFormat>();
  }
  if (!commandAck_) {
    commandAck_ = std::make_shared<CommunicationFormat>();
  }
  const bool success = currentCommand_->setData(m_sptr->payload);
  if (!success) {
    std::cerr << "Error in " << module_id() << "::mod_analyze: " << "Setting data to CommunicationFormat failed." << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqFormatErrorType(subsystem_, true));
    }
    return AS_OK;
  }
  commandIndex_++;
  currentCommand_->interpret();
  if (sendTelemetry_) {
    sendTelemetry_->setLastComIndex(subsystem_, commandIndex_);
    sendTelemetry_->setLastComCode(subsystem_, currentCommand_->Code());
  }
  if (chatter_ > 1) {
    std::cout << module_id() << ": Sending command:" << std::endl;
    currentCommand_->print(std::cout);
  }

  commandAck_->setCode(currentCommand_->Code());
  commandAck_->setArgc(1); // size of the command
  commandAck_->setArguments(0, currentCommand_->Command().size());
  commandAck_->update();
  const int send_result = socketCommunicationManager_->sendAndWaitForAck(m_sptr->payload, commandAck_->Command()); // TODO: this depends on telemetry definition.
  if (send_result < 0) {
    std::cerr << "Error in " << module_id() << "::mod_analyze: " << "Sending command is failed" << std::endl;
    //failed_ = true;
    commandRejectCount_++;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqComErrorType(subsystem_, true));
      sendTelemetry_->setCommandRejectedIndex(subsystem_, commandRejectCount_);
    }
  }
  else if (chatter_ > 0) {
    std::cout << module_id() << ": Sent data from " << m_sptr->topic << std::endl;
    std::cout << module_id() << ": Payload size: " << send_result << std::endl;
  }
  if (chatter_ > 1) {
    std::cout << module_id() << ": Payload:" << std::endl;
    for (const auto &byte: m_sptr->payload) {
      std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
  }
  return AS_OK;
}
bool SendCommandToDAQComputer::makeDAQEmergencyShutdownCommand() {
  if (!currentCommand_) {
    currentCommand_ = std::make_shared<CommunicationFormat>();
  }
  if (subsystem_ == Subsystem::ORC) {
    currentCommand_->setCode(castCommandCode(CommunicationCodes::ORC_Shutdown_All_DAQ));
    currentCommand_->setArgc(0);
    currentCommand_->update();
    return true;
  }
  else if (subsystem_ == Subsystem::COL) {
    currentCommand_->setCode(castCommandCode(CommunicationCodes::COL_Stop_Run));
    currentCommand_->setArgc(0);
    currentCommand_->update();
    return true;
  }
  else if (subsystem_ == Subsystem::TOF) {
    currentCommand_->setCode(castCommandCode(CommunicationCodes::TOF_Stop_DAQ));
    currentCommand_->setArgc(0);
    currentCommand_->update();
    return true;
  }
  else {
    return false;
  }
}

void SendCommandToDAQComputer::performDAQEmergencyShutdown() {
  if (chatter_ > 0) {
    std::cout << module_id() << ": Sending emergency DAQ shutdown command." << std::endl;
  }
  if (!socketCommunicationManager_) {
    std::cerr << module_id() << "::mod_analyze SocketCommunicationManager is nullptr." << std::endl;
    return;
  }
  if (!socketCommunicationManager_->isConnected()) {
    if (chatter_ > 1) {
      std::cout << module_id() << ": SocketCommunication is not connected." << std::endl;
    }
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqComErrorType(subsystem_, true));
    }
    //failed_ = true;
  }
  else {
    const int send_result = socketCommunicationManager_->sendAndWaitForAck(currentCommand_->Command(), commandAck_->Command());
    if (send_result < 0) {
      std::cerr << "Error in " << module_id() << "::mod_analyze: " << "Sending emergency DAQ shutdown command is failed" << std::endl;
      if (sendTelemetry_) {
        sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqComErrorType(subsystem_, true));
      }
    }
    else if (chatter_ > 0) {
      std::cout << "Sent emergency DAQ shutdown command" << std::endl;
    }
  }
  commandIndex_++;
  if (sendTelemetry_) {
    sendTelemetry_->setLastComIndex(subsystem_, commandIndex_);
    sendTelemetry_->setLastComCode(subsystem_, currentCommand_->Code());
  }
  return;
}

void SendCommandToDAQComputer::sendHeartbeatIfNeeded() {
  auto now = std::chrono::high_resolution_clock::now();
  const bool need_heartbeat = (!lastTime_) || (lastTime_ && (now - *lastTime_) > *durationBetweenHeartbeatChrono_);
  if (!lastTime_) {
    lastTime_ = std::make_shared<std::chrono::time_point<std::chrono::high_resolution_clock>>(now);
  }
  if (need_heartbeat) {
    heartbeat_->setArguments(0, heartbeat_->getArguments(0) + 1);
    heartbeat_->update();
    if (chatter_ > 1) {
      std::cout << "Sending heartbeat (index: " << heartbeat_->getArguments(0) << ")" << std::endl;
    }
    if (chatter_ > 2) {
      for (const auto &byte: heartbeat_->Command()) {
        std::cout << std::hex << static_cast<int>(byte) << std::dec << " ";
      }
      std::cout << std::endl;
    }
    const int send_result = socketCommunicationManager_->sendAndWaitForAck(heartbeat_->Command(), heartbeatAck_->Command());
    if (send_result < 0) {
      std::cerr << "Error in " << module_id() << "::mod_analyze: " << "Sending heartbeat is failed" << std::endl;
      if (sendTelemetry_) {
        sendTelemetry_->getErrorManager()->setError(ErrorManager::GetDaqFormatErrorType(subsystem_, true));
      }
    }
    else if (chatter_ > 1) {
      std::cout << "Sent heartbeat" << std::endl;
    }
    *lastTime_ = now;
  }
}

} // namespace gramsballoon::pgrams
