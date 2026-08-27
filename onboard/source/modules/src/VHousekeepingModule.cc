#include "VHousekeepingModule.hh"
#include <string>
using namespace anlnext;
namespace gramsballoon::pgrams {
const std::string VHousekeepingModule::evsStr_ = "HKLoop:ON";
VHousekeepingModule::VHousekeepingModule() = default;
VHousekeepingModule::~VHousekeepingModule() = default;

ANLStatus VHousekeepingModule::mod_begin_run() {
  isRateControlled_ = is_evs_defined(evsStr_);
  return AS_OK;
}

bool VHousekeepingModule::isInHKLoop() {
  return isRateControlled_ ? evs(evsStr_) : true;
}

} // namespace gramsballoon::pgrams