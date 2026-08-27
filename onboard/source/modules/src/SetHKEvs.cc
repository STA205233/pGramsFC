#include "SetHKEvs.hh"
#include <chrono>
#include <string>
using namespace anlnext;
using std::chrono::steady_clock;
namespace gramsballoon::pgrams {
const std::string SetHKEvs::evsStr_ = "HKLoop:ON";
ANLStatus SetHKEvs::mod_define() {
  define_parameter("duration_msec", &mod_class::durationmSec_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus SetHKEvs::mod_pre_initialize() {
  define_evs(evsStr_);
  lastTime_ = steady_clock::now();
  duration_ = std::chrono::milliseconds(durationmSec_);
  return AS_OK;
}
ANLStatus SetHKEvs::mod_analyze() {
  const auto now = steady_clock::now();
  if (now - lastTime_ > duration_) {
    set_evs(evsStr_);
    if (chatter_ > 0) {
      std::cout << "The HK data will be read in this loop" << std::endl;
    }
    lastTime_ = now;
  }
  if (chatter_ > 4) {
    std::cout << "The HK data will NOT be read in this loop" << std::endl;
  }
  return AS_OK;
}
} // namespace gramsballoon::pgrams