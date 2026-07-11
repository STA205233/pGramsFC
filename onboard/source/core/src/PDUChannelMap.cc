#include "PDUChannelMap.hh"
#include "CommunicationCodes.hh"
#include <utility>

namespace gramsballoon::pgrams {
using ::pgrams::communication::CommunicationCodes;
using ch_t = PDUChannelMap::ch_t;
using cs_msk_t = PDUChannelMap::cs_msk_t;
PDUChannelMap::PDUChannelMap() { construct(); }

bool PDUChannelMap::getMapping(com_code_t code, cs_msk_t& cs) const {
  auto it = mapping_.find(code);
  if (it == mapping_.end()) {
    return false;
  }
  cs = it->second;
  return true;
}

void PDUChannelMap::construct() {
  addOne(CommunicationCodes::PDU_SiPM0_VSET, 0);
  addOne(CommunicationCodes::PDU_SiPM1_VSET, 1);
  addOne(CommunicationCodes::PDU_SiPM2_VSET, 2);
  addOne(CommunicationCodes::PDU_SiPM3_VSET, 3);
  addOne(CommunicationCodes::PDU_SiPM4_VSET, 4);
  addOne(CommunicationCodes::PDU_SiPM5_VSET, 5);
  addOne(CommunicationCodes::PDU_PressureReg_VSET, 6);
}

void PDUChannelMap::addOne(CommunicationCodes code, cs_msk_t cs) {
  mapping_.emplace(to_u16(code), cs);
}
} // namespace gramsballoon::pgrams