#ifndef ReceiveTelemetry_H
#define ReceiveTelemetry_H 1

#include "MosquittoManager.hh"
#include <anlnext/BasicModule.hh>
#include <chrono>
#include <deque>
#include <thread>

namespace gramsballoon::pgrams {
template <typename T>
class MosquittoManager;

class ReceiveTelemetry: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(ReceiveTelemetry, 1.0);

public:
  ReceiveTelemetry() = default;
  virtual ~ReceiveTelemetry() = default;

protected:
  ReceiveTelemetry(const ReceiveTelemetry &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  auto Telemetry() { return telemetry_; }
  const auto &Telemetry() const { return telemetry_; }
  bool Valid() { return valid_; }
  void setValid(bool v) { valid_ = v; }
  int Chatter() { return chatter_; }

protected:
  void setTelemetry(const std::string &v) { telemetry_ = v; }

private:
  std::string telemetry_ = "";
  bool valid_;

  // communication
  MosquittoManager<std::string> *mosquittoManager_ = nullptr;
  MosquittoIO<std::string> *mosq_ = nullptr;
  int qos_ = 0;
  std::string subTopic_ = "Telemetry";
  int chatter_ = 0;
};

} // namespace gramsballoon::pgrams
#endif // ReceiveTelemetry_H
