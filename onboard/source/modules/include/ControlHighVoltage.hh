/**
 * Control high voltage via Analog Discovery.
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-15
 */


#ifndef ControlHighVoltage_H
#define ControlHighVoltage_H 1
#ifdef GB_DEMO_MODE
#include "ControlHighVoltageDemo.hh"
#else /* GB_DEMO_MODE */
#include <anlnext/BasicModule.hh>
#include "AnalogDiscoveryManager.hh"
#include "SendTelemetry.hh"

namespace gramsballoon {

class AnalogDiscoveryManager;
class SendTelemetry;

class ControlHighVoltage : public anlnext::BasicModule
{
  DEFINE_ANL_MODULE(ControlHighVoltage, 1.0);
  ENABLE_PARALLEL_RUN(); 

public:
  ControlHighVoltage();
  virtual ~ControlHighVoltage();
protected:
  ControlHighVoltage(const ControlHighVoltage& r) = default;

public:  
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  void setInvalidChannelError();

  bool setNextVoltage(double v);
  void setExec(bool v) { singleton_self()->exec_ = v; }
  double CurrentVoltage() { return singleton_self()->currentVoltage_; }
  double NextVoltage() { return singleton_self()->nextVoltage_; }
  double UpperLimitVoltage() { return singleton_self()->upperLimitVoltage_; }
  void setUpperLimitVoltage(double v) { singleton_self()->upperLimitVoltage_ = v; }

private:
  int deviceID_ = 0;
  int channel_ = 0;
  std::string ADManagerName_ = "AnalogDiscoveryManager";
  AnalogDiscoveryManager* ADManager_ = nullptr;
  int sleep_ = 500; // ms
  double currentVoltage_ = 0.0; // volt
  double nextVoltage_ = 0.0; // volt
  bool exec_ = false;
  std::vector<double> voltages_;
  int voltageIndex_ = 0;
  SendTelemetry* sendTelemetry_ = nullptr;
  double upperLimitVoltage_ = 0.0;
  int chatter_ = 0;
};

} /* namespace gramsballoon */

#endif /* GB_DEMO_MODE */
#endif /* ControlHighVoltage_H */
