#ifndef GB_VHouseKeepingModule_hh
#define GB_VHouseKeepingModule_hh 1

#include "anlnext/BasicModule.hh"
#include <string>
namespace gramsballoon::pgrams {

/**
 * @brief Virtual class for house keeping data taking modules
 * @author Shota Arai
 * @date 2026-08-28 | Shota Arai | First version (v1.0)
 */
class VHousekeepingModule: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(VHousekeepingModule, 1.0)
  ENABLE_PARALLEL_RUN()
public:
  VHousekeepingModule();
  virtual ~VHousekeepingModule();

protected:
  bool isInHKLoop();
  anlnext::ANLStatus mod_begin_run() override;

private:
  bool isRateControlled_;
  const static std::string evsStr_;
};
} // namespace gramsballoon::pgrams
#endif //GB_VHouseKeepingModule_hh