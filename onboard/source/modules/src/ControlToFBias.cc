#include "ControlToFBias.hh"
#include "CommunicationCodes.hh"
#include "MosquittoManager.hh"
#include "SendTelemetry.hh"
#include "ToFBiasController.hh"
#include "ToFBiasTelemetry.hh"
#include <cstdint>
using namespace anlnext;
using ::pgrams::communication::TelemetryCodes;
using ::pgrams::communication::to_telem_u16;
namespace gramsballoon::pgrams {
ControlToFBias::ControlToFBias() : mosquittoManager_(nullptr), fullPacketStatus_(FullOutputStatus::WAITING), controller_(nullptr), timeout_(50000) {}

anlnext::ANLStatus ControlToFBias::mod_define() {
  define_parameter("path", &mod_class::path_);
  define_parameter("MosquittoManager_name", &mod_class::mosquittoManagerName_);
  define_parameter("chatter", &mod_class::chatter_);
  define_parameter("minimum_duration_sec", &mod_class::minDurationSec_);
  define_parameter("starlink_topic", &mod_class::starlinkTopic_);
  define_parameter("topic", &mod_class::topic_);
  define_parameter("timeout_usec", &mod_class::timeout_);
  return AS_OK;
}

ANLStatus ControlToFBias::mod_initialize() {
  if (exist_module(mosquittoManagerName_)) {
    get_module_IFNC(mosquittoManagerName_, &mosquittoManager_);
  }

  if (exist_module("SendTelemetry")) {
    get_module_IFNC("SendTelemetry", &sendTelemetry_);
  }

  duration_ = std::chrono::seconds(minDurationSec_);
  index_ = 0;
  telemetryStr_.reserve(1000);

  telem_ = std::make_shared<ToFBiasTelemetry>(true);
  controller_ = std::make_shared<ToFBiasController>(path_);
  controller_->setTimeout(std::chrono::microseconds(timeout_));
  const int init_res = controller_->initialize();
  if (init_res != 0) {
    std::cerr << "Error in opening file " << path_ << std::endl;
    treatError();
    return AS_OK;
  }
  return AS_OK;
}

ANLStatus ControlToFBias::mod_begin_run() {
  return AS_OK;
}
ANLStatus ControlToFBias::mod_end_run() {
  if (controller_) {
    const int ret = controller_->disableDataStream();
    if (ret < 0) {
      treatError();
      return AS_OK;
    }
  }
  return AS_OK;
}
ANLStatus ControlToFBias::mod_analyze() {
  if (!controller_) {
    return AS_OK;
  }
  if (controller_->HasError()) {
    const int ret = controller_->initialize();
    if (ret < 0) {
      treatError();
      return AS_OK;
    }
  }

  // For full output status
  if (fullPacketStatus_ == FullOutputStatus::REQUESTING) { // Requested from commands
    const int ret = controller_->queryFullOutput();
    if (ret < 0) {
      treatError();
      return AS_OK;
    }
    fullPacketStatus_ = FullOutputStatus::REQUESTED;
  }
  else if (fullPacketStatus_ == FullOutputStatus::REQUESTED) {
    const int ret = controller_->getFullOutput();
    telemetryStr_.clear();
    telemetryStr_ = controller_->getData();
    if (ret < 0) {
      treatError();
      return AS_OK;
    }
    sendPacket(telemetryStr_, TelemetryCodes::HUB_Tof_Bias_full);
    lastReceivedTime_ = std::chrono::steady_clock::now(); // reset counter for telemetry
    return AS_OK;
  }

  const auto now = std::chrono::steady_clock::now();
  if (now - lastReceivedTime_ > duration_) {
    telemetryStr_.clear();
    const int packet_result = controller_->getOnePacket(telemetryStr_);
    if (packet_result >= 0) {
      if (mosquittoManager_) {
        sendPacket(telemetryStr_, TelemetryCodes::HUB_Tof_Bias_summary);
      }
    }
    else {
      treatError();
    }
    lastReceivedTime_ = now;
    return AS_OK;
  }
  return AS_OK;
}
ANLStatus ControlToFBias::mod_finalize() {
  singleton_self()->controller_.reset();
  return AS_OK;
}

int ControlToFBias::setVoffset(uint32_t voltage) {
  const auto ret = singleton_self()->controller_->setVoffset(voltage);
  if (ret < 0) {
    treatError();
  }
  return ret;
}

int ControlToFBias::setTmuxChannel(uint32_t channel, int onOff) {
  const auto ret = singleton_self()->controller_->setTmuxChannel(channel, onOff);
  if (ret < 0) {
    if (singleton_self()->sendTelemetry_) {
      singleton_self()->sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
    }
  }
  return ret;
}

int ControlToFBias::enableDCDC(uint32_t channel) {
  const int ret = singleton_self()->controller_->enableDCDC(channel);
  if (ret < 0) {
    if (singleton_self()->sendTelemetry_) {
      singleton_self()->sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
    }
  }
  return ret;
}
int ControlToFBias::disableDCDC(uint32_t channel) {
  const int ret = singleton_self()->controller_->disableDCDC(channel);
  if (ret < 0) {
    if (singleton_self()->sendTelemetry_) {
      singleton_self()->sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
    }
  }
  return ret;
}

int ControlToFBias::setVdef(uint32_t channel, uint32_t voltage) {
  const int ret = singleton_self()->controller_->setVdef(channel, voltage);
  if (ret < 0) {
    if (singleton_self()->sendTelemetry_) {
      singleton_self()->sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
    }
  }
  return ret;
}

void ControlToFBias::sendPacket(const std::string &str, TelemetryCodes code) {
  const auto &topic = (mosquittoManager_->getLinkType() == CommunicationLinkType::STARLINK) ? starlinkTopic_ : topic_;
  telem_->setType(Subsystem::HUB);
  const auto &cont_sptr = telem_->getContentsSptr();
  telem_->setArguments(str);
  telemetryStr_.clear();
  cont_sptr->setCode(to_telem_u16(code));
  telem_->setIndex(index_);
  telem_->setCurrentTime();
  telem_->construct(telemetryStr_);
  mosquittoManager_->Publish(telemetryStr_, topic, qos_);
  ++index_;
}

void ControlToFBias::treatError() {
  if (singleton_self()->sendTelemetry_) {
    singleton_self()->sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
  }
}

int ControlToFBias::queryFullOutput() {
  if (singleton_self()->fullPacketStatus_ != FullOutputStatus::WAITING) {
    std::cerr << module_id() << ": Full Output is already requested" << std::endl;
    return -1;
  }
  singleton_self()->fullPacketStatus_ = FullOutputStatus::REQUESTING;
  return 0;
}
} // namespace gramsballoon::pgrams