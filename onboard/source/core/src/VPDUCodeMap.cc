#include "VPDUCodeMap.hh"
#include "CommunicationCodes.hh"
#include <utility>

namespace gramsballoon::pgrams {
using ::pgrams::communication::CommunicationCodes;
using value_t = VPDUCodeMap::value_t;

bool VPDUCodeMap::getMapping(com_code_t code, value_t& cs) const {
  auto it = mapping_.find(code);
  if (it == mapping_.end()) {
    return false;
  }
  cs = it->second;
  return true;
}

void VPDUCodeMap::addOne(CommunicationCodes code, value_t cs) {
  mapping_.emplace(to_u16(code), cs);
}
} // namespace gramsballoon::pgrams