#ifndef GB_SetHKEvs_hh
#define GB_SetHKEvs_hh 1
#include "anlnext/BasicModule.hh"
#include <chrono>

namespace gramsballoon::pgrams {

/**
 * @brief Class to manage time for Housekeeping data taking
 * @author Shota Arai
 * @date 2026-08-28 | Shota Arai | First version (v1.0)
 */
class SetHKEvs: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(SetHKEvs, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  SetHKEvs() = default;
  virtual ~SetHKEvs() = default;

protected:
  SetHKEvs(const SetHKEvs &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_pre_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

private:
  const static std::string evsStr_;
  int durationmSec_ = 1000;
  int chatter_ = 0;
  std::chrono::milliseconds duration_;
  std::chrono::steady_clock::time_point lastTime_;
};
} // namespace gramsballoon::pgrams
#endif // GB_SetHKEvs_hh