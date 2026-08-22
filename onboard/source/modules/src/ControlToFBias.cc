#include "ControlToFBias.hh"
#include "BaseTelemetryDefinition.hh"
#include "CommunicationCodes.hh"
#include "CommunicationFormat.hh"
#include "MosquittoManager.hh"
#include "SendTelemetry.hh"
#include "ToFBiasController.hh"
#include <cstdint>
using namespace anlnext;
namespace gramsballoon::pgrams {
ControlToFBias::ControlToFBias() : mosquittoManager_(nullptr), controller_(nullptr), timeout_(50000) {}

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

  telem_ = std::make_shared<BaseTelemetryDefinition>(true);
  controller_ = std::make_shared<ToFBiasController>(path_);
  controller_->setTimeout(std::chrono::microseconds(timeout_));
  const int init_res = controller_->initialize();
  if (init_res != 0) {
    std::cerr << "Error in opening file " << path_ << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
    }
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
      if (sendTelemetry_) sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
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
      if (sendTelemetry_) sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
      return AS_OK;
    }
  }
  const auto now = std::chrono::steady_clock::now();
  if (now - lastReceivedTime_ > duration_) {
    telemetryStr_.clear();
    const int packet_result = controller_->getOnePacket(telemetryStr_);
    if (packet_result >= 0) {
      if (mosquittoManager_) {
        const auto &topic = (mosquittoManager_->getLinkType() == CommunicationLinkType::STARLINK) ? starlinkTopic_ : topic_;
        telem_->setType(Subsystem::HUB);
        const auto &cont_sptr = telem_->getContentsSptr();
        fillPacket(cont_sptr.get(), telemetryStr_);
        telemetryStr_.clear();
        cont_sptr->setCode(::pgrams::communication::to_telem_u16(::pgrams::communication::TelemetryCodes::HUB_Telemetry_Normal));
        telem_->setIndex(index_);
        telem_->setCurrentTime();
        telem_->construct(telemetryStr_);
        mosquittoManager_->Publish(telemetryStr_, topic, qos_);
        ++index_;
      }
    }
    else {
      if (sendTelemetry_) {
        sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
      }
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
    if (singleton_self()->sendTelemetry_) {
      singleton_self()->sendTelemetry_->getErrorManager()->setError(ErrorType::TOF_BIAS_COM_ERROR);
    }
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

void ControlToFBias::fillPacket(CommunicationFormat *com, const std::string &str) {
  const int sz_str = str.size();
  const int sz = (sz_str + 3) / 4;
  com->setArgc(sz);
  auto d = str.data();
  for (int i = 0; i < sz; ++i) {
    uint32_t v = 0;
    for (int j = 0; j < 4; ++j) {
      const int index = i * 4 + j;
      uint32_t vv = index < sz_str ? static_cast<uint8_t>(d[index]) : 0;
      v |= (vv << (24 - j * 8));
    }
    com->setArguments(i, v);
  }
}

} // namespace gramsballoon::pgrams