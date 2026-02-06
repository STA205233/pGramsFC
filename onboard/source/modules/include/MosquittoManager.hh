#ifndef GB_MosquittoManager_hh
#define GB_MosquittoManager_hh 1
#include "ErrorManager.hh"
#include "MosquittoIO.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
#include <string>

namespace gramsballoon::pgrams {
enum class CommunicationLinkType : uint32_t {
  IRIDIUM = 0,
  STARLINK = 1,
};
class SendTelemetry;

/**
 * Module for managing Mosquitto
 * @author Shota Arai
 * @date 2024-**-** Shota Arai| First implementation.
 * @date 2025-09-20 Shota Arai| Changed to template class to handle different types of telemetry. (v2.0)
 * @date 2026-02-05 Shota Arai| Added doInitialize parameter to control mosqpp::lib_init and lib_cleanup calls. (v2.1)
**/

template <typename T>
class MosquittoIO;
template <typename TelemType>
class MosquittoManager: public anlnext::BasicModule {
  using mod_class = MosquittoManager<TelemType>;
  std::string __module_name__() const override {
    if constexpr (std::is_same_v<TelemType, std::string>)
      return "TelemMosquittoManager";
    else if constexpr (std::is_same_v<TelemType, std::vector<uint8_t>>)
      return "ComMosquittoManager";
  }
  std::string __module_version__() const override { return "2.1"; }
  ENABLE_PARALLEL_RUN();

public:
  MosquittoManager() = default;
  virtual ~MosquittoManager() {
    if (doInitialize_) { 
      mosqpp::lib_cleanup();
    }
    mosquittoIO_.reset();
  }

protected:
  MosquittoManager(const MosquittoManager<TelemType> &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_pre_initialize() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_begin_run() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_end_run() override;
  anlnext::ANLStatus mod_finalize() override;

  void Publish(TelemType &message, const std::string &topic, int qos) {
    singleton_self()->mosquittoIO_->Publish(message, topic, qos);
  }
  MosquittoIO<TelemType> *getMosquittoIO() { return singleton_self()->mosquittoIO_.get(); }
  CommunicationLinkType getLinkType() const { return singleton_self()->linkType_; }
  void setLinkType(CommunicationLinkType v) { singleton_self()->linkType_ = v; }

private:
  std::shared_ptr<MosquittoIO<TelemType>> mosquittoIO_ = nullptr;
  std::string host_ = "localhost";
  std::string user_ = "";
  std::string passwd_ = "";
  std::string deviceId_ = "MosquittoManager";
  SendTelemetry *sendTelemetry_ = nullptr;
  int port_ = 1883;
  int keepAlive_ = 60;
  int chatter_ = 0;
  int timeout_ = 10;
  bool doInitialize_ = false;
  bool threadedSet_ = true;
  anlnext::ANLStatus HandleError(int error_code);
  CommunicationLinkType linkType_ = CommunicationLinkType::IRIDIUM;
};
} // namespace gramsballoon::pgrams
#endif // GB_MosquittoManager_hh