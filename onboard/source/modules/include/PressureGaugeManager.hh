#ifndef GRAMSBalloon_PressureGaugeManager_hh
#define GRAMSBalloon_PressureGaugeManager_hh 1
#include "EncodedSerialCommunicator.hh"
#include <string>
#include <string_view>

namespace gramsballoon::pgrams {
class PressureGaugeManager: public EncodedSerialCommunicator {
  DEFINE_ANL_MODULE(PressureGaugeManager, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  PressureGaugeManager() = default;
  virtual ~PressureGaugeManager() = default;

protected:
  PressureGaugeManager(const PressureGaugeManager &r) = default;

public:
  int SendComAndGetData(std::string_view command, std::string &data, int max_length) override;
};
} // namespace gramsballoon::pgrams

#endif //GRAMSBalloon_PressureGaugeManager_hh