#ifndef ShutdownSystem_H
#define ShutdownSystem_H 1
#ifdef USE_SYSTEM_MODULES
/**
 * Test module for shutdown system
 * @author Shota Arai, Tsubasa Tamba
 * @date 2023-04-16
 */

#include "SendTelemetry.hh"
#include <anlnext/BasicModule.hh>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <sys/time.h>

namespace gramsballoon::pgrams {
class SendTelemetry;
class ShutdownSystem: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(ShutdownSystem, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  ShutdownSystem() = default;
  virtual ~ShutdownSystem() = default;

protected:
  ShutdownSystem(const ShutdownSystem &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;
  void setReboot(bool v);
  void setShutdown(bool v);
  void setSoftwareStop(bool v);
  void setPrepareReboot(bool v);
  void setPrepareShutdown(bool v);
  void setPrepareSoftwareStop(bool v);
  void clearStatus();
  SendTelemetry *getSendTelemetry() { return singleton_self()->sendTelemetry_; }
  void setExitStatus(int v) { singleton_self()->exitStatus_ = v; }

private:
  bool reboot_ = false;
  bool shutdown_ = false;
  bool softwareStop_ = false;
  bool prepareReboot_ = false;
  bool prepareShutdown_ = false;
  bool prepareSoftwareStop_ = false;
  int bufferTimeSec_ = 60;
  timeval prepareRebootTime_;
  timeval prepareShutdownTime_;
  timeval prepareSoftwareStopTime_;
  SendTelemetry *sendTelemetry_ = nullptr;
  int exitStatus_ = 0;
  int chatter_ = 0;
};

} /* namespace gramsballoon */
#endif /* USE_SYSTEM_MODULES */
#endif /*ShutdownSystem_H*/
