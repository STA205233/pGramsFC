#ifndef ReceiveCommand_H
#define ReceiveCommand_H 1

#include "CommunicationFormat.hh"
#include "CommunicationSaver.hh"
#include "ControlToFBias.hh"
#include "MosquittoManager.hh"
#include "RunIDManager.hh"
#ifdef USE_SPI
#include "SPIManager.hh"
#endif
#include "SendTelemetry.hh"
#include <anlnext/BasicModule.hh>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace gramsballoon {
class RunIDManager;
namespace pgrams {
class ShutdownSystem;
class SendTelemetry;
template <typename T>
class CommunicationSaver;
template <typename T>
class MosquittoManager;
template <typename T>
class MosquittoIO;
class SendCommandToDAQComputer;
class ControlToFBias;

#ifdef USE_SPI
class PDUChannelMap;
class ControlPDU;
class SPIManager;
class PDUCodeMapDIO;
class PDUCodeMapCS;
#endif
/**
 * Receive commands from ground.
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 * @date 2024-11-28 Shota Arai| Modified for pGRAMS telemetry and command system.
 * @date 2025-12-14 Shota Arai| Added access to SendCommandToDAQComputer modules and emergency DAQ shutdown feature. (v1.1)
 */
class ReceiveCommand: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(ReceiveCommand, 1.1);
  ENABLE_PARALLEL_RUN();

public:
  ReceiveCommand();
  virtual ~ReceiveCommand();

protected:
  ReceiveCommand(const ReceiveCommand& r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  uint16_t CommandCode() { return (singleton_self()->comdef_)->Code(); }
  uint32_t CommandIndex() { return singleton_self()->commandIndex_; }
  uint16_t CommandRejectCount() { return singleton_self()->commandRejectCount_; }

private:
  void getModules();
  bool applyCommand(const std::vector<uint8_t>& command);
  #ifdef USE_SPI
  bool applySPICommand(uint16_t code, const uint16_t argc, const std::vector<uint32_t>& arguments);
  #endif

  std::shared_ptr<pgrams::CommunicationFormat> comdef_ = nullptr;
  uint32_t commandIndex_ = 0;
  uint16_t commandRejectCount_ = 0;

  bool saveCommand_ = true;
  std::string binaryFilenameBase_ = "";
  int numCommandPerFile_ = 100;
  int chatter_ = 0;

  // access to other classes
  SendTelemetry *sendTelemetry_ = nullptr;
#ifdef USE_SYSTEM_MODULES
  ShutdownSystem *shutdownSystem_ = nullptr;
#endif
  RunIDManager *runIDManager_ = nullptr;
  MosquittoManager<std::vector<uint8_t>> *mosquittoManager_ = nullptr;
  MosquittoManager<std::string> *telemetryMosquittoManager_ = nullptr;
  ControlToFBias *controlTofBias_ = nullptr;
#ifdef USE_SPI
  SPIManager *spiManager_ = nullptr;
  ControlPDU *controlPDU_ = nullptr;
  #endif
  std::string spiManagerName_ = "SPIManager";

  // communication
  MosquittoIO<std::vector<uint8_t>> *mosq_ = nullptr;
  std::string topic_ = "command";
  int qos_ = 0;
  int timeoutSec_ = 2;

  std::shared_ptr<CommunicationSaver<std::vector<uint8_t>>> commandSaver_ = nullptr;
  std::vector<SendCommandToDAQComputer *> sendCommandToDAQComputers_;
  std::vector<std::string> sendCommandToDAQComputerNames_;
#ifdef USE_SPI
  PDUCodeMapCS& pduCodeMapCS_;
  PDUCodeMapDIO& pduCodeMapDIO_;
  #endif
};

} // namespace pgrams
} /* namespace gramsballoon */

#endif /* ReceiveCommand_H */
