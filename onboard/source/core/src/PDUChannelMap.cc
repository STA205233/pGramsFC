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
  addOne(CommunicationCodes::PDU_SiPM0_VSET, 10);
  addOne(CommunicationCodes::PDU_SiPM1_VSET, 11);
  addOne(CommunicationCodes::PDU_SiPM2_VSET, 12);
  addOne(CommunicationCodes::PDU_SiPM3_VSET, 13);
  addOne(CommunicationCodes::PDU_SiPM4_VSET, 14);
  addOne(CommunicationCodes::PDU_SiPM5_VSET, 15);
  addOne(CommunicationCodes::PDU_PressureReg_VSET, 16);
}

void PDUChannelMap::addOne(CommunicationCodes code, cs_msk_t cs) {
  mapping_.emplace(to_u16(code), cs);
}
} // namespace gramsballoon::pgrams