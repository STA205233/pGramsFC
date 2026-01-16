/**
 * GetComputerStatus sample module of the ANL Next framework
 *
 * @author 
 * @date
 *
 */

#ifndef GetRaspiStatus_H
#define GetRaspiStatus_H 1
#ifdef USE_SYSTEM_MODULES
#include "SendTelemetry.hh"
#include <anlnext/BasicModule.hh>
#include <cstdint>

namespace gramsballoon::pgrams {

class SendTelemetry;

class GetComputerStatus: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(GetComputerStatus, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  GetComputerStatus();
  virtual ~GetComputerStatus();

protected:
  GetComputerStatus(const GetComputerStatus &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  int getCapacity();
  int getRAMUsage();

  int CPUTemperatureADC() const { return singleton_self()->CPUTemperatureADC_; }
  double CPUTemperature() const { return singleton_self()->CPUTemperature_; }
  uint64_t CapacityFree() const { return singleton_self()->capacityFree_; }
  uint64_t CapacityAll() const { return singleton_self()->capacityAll_; }
  uint64_t MemoryUsage() const { return singleton_self()->ramUsage_; }

private:
  int CPUTemperatureADC_;
  double CPUTemperature_;
  std::string tempFile_;
  std::string memFile_;
  std::string path_;
  uint64_t capacityFree_ = 0;
  uint64_t capacityAll_ = 0;
  uint64_t ramUsage_ = 0;
  uint64_t ramAvail_ = 0;
  uint64_t ramTotal_ = 0;
  SendTelemetry *sendTelemetry_ = nullptr;
  int chatter_ = 0;
};

} // namespace gramsballoon::pgrams
#endif // USE_SYSTEM_MODULES
#endif /* GetRaspiStatus_H */
