#ifndef GB_DistributeCommand_hh
#define GB_DistributeCommand_hh 1
#include "MosquittoManager.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
#include "sys/socket.h"
#include <arpa/inet.h>
#include <map>
#include <string>
namespace gramsballoon {
class SendTelemetry;
} // namespace gramsballoon
namespace gramsballoon::pgrams {
class MosquittoManager;
struct SubSystem {
  int socket = 0;
  sockaddr_in serverAddress;
  int port = 0;
  std::string ip = "";
  std::string topic;
  SubSystem(int s, int p, const std::string &i, const std::string &t) : socket(s), port(p), ip(i), topic(t) {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
  }
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
  bool IsFailed() const { return singleton_self()->failed_; }

private:
  MosquittoManager *mosquittoManager_ = nullptr;
  MosquittoIO<std::string> *mosq_ = nullptr;
  std::map<const std::string, SubSystem, std::less<>> subSystems_;
  SendTelemetry *sendTelemetry_ = nullptr;
  int chatter_ = 0;
  int numTrial_ = 10;
  bool failed_ = false;
};
} // namespace gramsballoon::pgrams
#endif // DistributeCommand_hh