/**
 * Receive commands from ground.
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 */


#ifndef RunIDManager_H
#define RunIDManager_H 1

#include <anlnext/BasicModule.hh>
#include <sys/time.h>

namespace gramsballoon::pgrams {
class SendTelemetry;
class RunIDManager : public anlnext::BasicModule
{
  DEFINE_ANL_MODULE(RunIDManager, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  RunIDManager();
  virtual ~RunIDManager();
protected:
  RunIDManager(const RunIDManager& r) = default;

public:  
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_pre_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  int RunID() const { return singleton_self()->runID_; }
  std::string TimeStampStr() const { return singleton_self()->timeStampStr_; }

private:
  int runID_ = 0;
  std::string filename_;
  std::string timeStampStr_;
  int chatter_;

};

} /* namespace gramsballoon */

#endif /* RunIDManager_H */
