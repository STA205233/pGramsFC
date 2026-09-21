#include "VPDUCodeMap.hh"
#include "CommunicationCodes.hh"
#include <iostream>
#include <utility>
namespace gramsballoon::pgrams {
using ::pgrams::communication::CommunicationCodes;
using value_t = VPDUCodeMap::value_t;

bool VPDUCodeMap::getMapping(com_code_t code, value_t &cs, bool &inversed) const {
  auto it = mapping_.find(code);
  if (it == mapping_.end()) {
    std::cout << "VPDUCodeMap: code(" << code << ") is not found" << std::endl;
    return false;
  }
  cs = it->second.first;
  inversed = it->second.second;
  return true;
}

bool VPDUCodeMap::getMapping(com_code_t code, value_t &cs) const {
  bool placeholder = true;
  return getMapping(code, cs, placeholder);
}

void VPDUCodeMap::addOne(CommunicationCodes code, value_t cs, bool inversed) {
  mapping_.emplace(to_u16(code), std::make_pair(cs, inversed));
}
} // namespace gramsballoon::pgrams