#include "DividePacket.hh"
#include <iomanip>
using namespace anlnext;
namespace gramsballoon::pgrams {
DividePacket::DividePacket() {
  iridiumPacketQueue_ = std::make_shared<IridiumPacketPool>();
}

ANLStatus DividePacket::mod_define() {
  define_parameter("ReceiveStatusFromDAQComputer_name", &mod_class::receiveStatusFromDAQComputerName_);
  set_parameter_description("Name of ReceiveStatusFromDAQComputer");
  define_parameter("starlink_code", &mod_class::starlinkCode_);
  define_parameter("overwritten_packet_code", &mod_class::overwrittenPacketCode_);
  set_parameter_description("Packet codes for overwriting packets");
  define_parameter("maxPacketSize", &mod_class::maxPacketSize_);
  set_parameter_description("Maximum packet size to publish (in bytes)");
  define_parameter("save_status", &mod_class::saveStatus_);
  define_parameter("binary_filename_base", &mod_class::binaryFilenameBase_);
  set_parameter_description("Base name for the binary files to save the received status.");
  define_parameter("num_status_per_file", &mod_class::numStatusPerFile_);
  set_parameter_description("Number of status messages to save per binary file.");
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus DividePacket::mod_initialize() {
  if (exist_module(receiveStatusFromDAQComputerName_)) {
    get_module_NC(receiveStatusFromDAQComputerName_, &receiveStatusFromDAQComputer_);
    subsystem_ = receiveStatusFromDAQComputer_->getSubsystem();
  }
  else {
    std::cerr << "ReceiveStatusFromDAQComputer module is not found." << std::endl;
    return AS_ERROR;
  }
  index_ = 0;
  if (saveStatus_) {
    statusSaver_ = std::make_shared<CommunicationSaver<std::vector<uint8_t>>>();
  }
  if (statusSaver_) {
    if (exist_module("RunIDManager")) {
      const RunIDManager *runIDManager = nullptr;
      get_module("RunIDManager", &runIDManager);
      if (statusSaver_) {
        statusSaver_->setRunID(runIDManager->RunID());
        statusSaver_->setTimeStampStr(runIDManager->TimeStampStr());
      }
    }
    statusSaver_->setBinaryFilenameBase(binaryFilenameBase_);
    statusSaver_->setNumCommandPerFile(numStatusPerFile_);
  }

  return AS_OK;
}
ANLStatus DividePacket::mod_analyze() {
  if (!receiveStatusFromDAQComputer_) {
    std::cerr << "ReceiveStatusFromDAQComputer is nullptr." << std::endl;
    return AS_OK;
  }
  uint16_t argc = 0;
  for (int i = 0; i < MAX_BYTES; i++) {
    if (receiveStatusFromDAQComputer_->GetBufferSize() == 0) {
      break;
    }
    const auto byte = receiveStatusFromDAQComputer_->PopAndGetOneByte();
    const auto sz = currentPacket_.size();
    if (chatter_ > 3) {
      std::cout << "byte: " << std::hex << static_cast<int>(byte) << std::dec << std::endl;
    }
    if (byte == 0xEB && sz == 0) {
      currentPacket_.push_back(byte);
    }
    else if (byte == 0x90 && sz == 1) {
      currentPacket_.push_back(byte);
    }
    else if (byte == 0x5B && sz == 2) {
      currentPacket_.push_back(byte);
    }
    else if (byte == 0x6A && sz == 3) {
      currentPacket_.push_back(byte);
    }
    else if (sz == 4) {
      currentCode_ = byte << 8;
      currentPacket_.push_back(byte);
    }
    else if (sz == 5) {
      currentCode_ |= byte;
      currentPacket_.push_back(byte);
    }
    else if (sz == 6) {
      argc = byte << 8;
      currentPacket_.push_back(byte);
    }
    else if (sz == 7) {
      argc |= byte;
      currentPacket_.push_back(byte);
      lastPacketSize_ = 14 + 4 * argc;
      std::cout << "packet_size: " << static_cast<int>(lastPacketSize_) << std::endl;
    }
    else if ((sz > 4 && sz < lastPacketSize_ - 4) || (sz == 4) || (sz == 5)) {
      currentPacket_.push_back(byte);
    }
    else if (sz == lastPacketSize_ - 4 && byte == 0xC5) {
      currentPacket_.push_back(byte);
    }
    else if (sz == lastPacketSize_ - 3 && byte == 0xA4) {
      currentPacket_.push_back(byte);
    }
    else if (sz == lastPacketSize_ - 2 && byte == 0xD2) {
      currentPacket_.push_back(byte);
    }
    else if (sz == lastPacketSize_ - 1 && byte == 0x79) {
      currentPacket_.push_back(byte);
      if (chatter_ > 2) {
        std::cout << "Divided packet: ";
        for (const auto &byte: currentPacket_) {
          std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
      }
      if (statusSaver_ && saveStatus_) {
        statusSaver_->writeCommandToFile(false, currentPacket_);
      }
      PushCurrentVector();
      currentPacket_.clear();
      lastPacketSize_ = 0;
      inError_ = false;
    }
    else if (inError_) {
      currentPacket_.push_back(byte);
      size_t new_sz = currentPacket_.size();
      if (new_sz >= 4 && currentPacket_[new_sz - 4] == 0xC5 && currentPacket_[new_sz - 3] == 0xA4 && currentPacket_[new_sz - 2] == 0xD2 && currentPacket_[new_sz - 1] == 0x79) {
        if (statusSaver_ && saveStatus_) {
          statusSaver_->writeCommandToFile(true, currentPacket_);
        }
        PushCurrentVector();
        currentPacket_.clear();
        lastPacketSize_ = 0;
        inError_ = false;
      }
    }
    else {
      std::cerr << "DividePacket::mod_analyze: Error in packet division. Finding next footer..." << std::endl;
      inError_ = true;
      currentPacket_.push_back(byte);
    }
  }
  return AS_OK;
}
ANLStatus DividePacket::mod_finalize() {
  return AS_OK;
}
void DividePacket::PushCurrentVector() {
  const auto size_packet = currentPacket_.size();
  if (size_packet > maxPacketSize_) {
    std::cerr << "DividePacket::PushCurrentVector: Packet size (" << size_packet << " bytes) is larger than the maximum size (" << maxPacketSize_ << " bytes). Skipping this packet." << std::endl;
    return;
  }
  auto telem = std::make_shared<BaseTelemetryDefinition>(true);
  telem->getContentsNC()->setCommand(currentPacket_);
  telem->setCurrentTime();
  index_++;
  telem->setIndex(index_);
  telem->setType(subsystem_);
  telem->update();
  for (const auto &code: starlinkCode_) {
    if (currentCode_ == code) {
      starlinkPacketQueue_.push(telem);
      if (chatter_ > 1) {
        std::cout << "DividePacket::PushCurrentVector: Pushed a Starlink packet. Queue size: " << starlinkPacketQueue_.size() << std::endl;
      }

      return;
    }
  }
  if (currentCode_ == overwrittenPacketCode_) {
    if (chatter_ > 0) {
      std::cout << "DividePacket::PushCurrentVector: Overwriting packet with code " << overwrittenPacketCode_ << std::endl;
    }
    iridiumPacketQueue_->push(telem, true);
    return;
  }
  iridiumPacketQueue_->push(telem, false);

  if (chatter_ > 1) {
    std::cout << "DividePacket::PushCurrentVector: Pushed an Iridium packet. Queue size: " << iridiumPacketQueue_->size() << std::endl;
  }
}
} // namespace gramsballoon::pgrams