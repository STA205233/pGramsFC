#ifndef DistrobuteCommand_hh
#define DistrobuteCommand_hh 1
#include "MosquittoManager.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
#include "sys/socket.h"
#include <map>
#include <string>

namespace gramsballoon {
class SendTelemetry;
} // namespace gramsballoon
namespace gramsballoon::pgrams {
class MosquittoManager;
struct SubSystem {
  int socket = 0;
  int port = 0;
  std::string ip = "";
  std::string topic;
  SubSystem(int s, int p, const std::string &i, const std::string &t) : socket(s), port(p), ip(i), topic(t) {}
};

class DistributeCommand: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(DistributeCommand, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  DistributeCommand();
  virtual ~DistributeCommand() = default;

protected:
  DistributeCommand(const DistributeCommand &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;
  bool IsFailed() const { return failed_; }

private:
  MosquittoManager *mosquittoManager_ = nullptr;
  MosquittoIO<std::vector<uint8_t>> *mosq_ = nullptr;
  std::map<std::string, SubSystem> subSystems_;
  SendTelemetry *sendTelemetry_ = nullptr;
  int chatter_ = 0;
  int numTrial_ = 0;
  bool failed_ = false;
};
} // namespace gramsballoon::pgrams
#endif // DistributeCommand_hh