#ifndef GB_GetInclinometerData_hh
#define GB_GetInclinometerData_hh 1
#include "InclinometerController.hh"
#include "SendTelemetry.hh"
#include "anlnext/BasicModule.hh"
namespace gramsballoon::pgrams {
/**
 * A class to get inclinometer data and output it as an ANL module
 *
 * @author Shota Arai
 * @date 2026-04-06 | Shota Arai | First version
 */
class GetInclinometerData: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(GetInclinometerData, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  GetInclinometerData() = default;
  virtual ~GetInclinometerData() = default;

protected:
  GetInclinometerData(const GetInclinometerData &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

private:
  std::string serialPath_ = "/dev/ttyS0";
  speed_t baudrate_ = B38400;
  int chatter_ = 0;
  std::shared_ptr<InclinometerController> inclinometerController_;
  SendTelemetry *sendTelemetry_ = nullptr;
  int timeoutSec_ = 0; // default timeout 100 ms
  int timeoutUsec_ = 100000; // default timeout 100 ms
};
} // namespace gramsballoon::pgrams

#endif //GB_GetInclinometerData_hh