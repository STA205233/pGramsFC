#include "PDUCodeMapCS.hh"
#include "CommunicationCodes.hh"

namespace gramsballoon::pgrams {
using ::pgrams::communication::CommunicationCodes;
using value_t = PDUCodeMapCS::value_t;

PDUCodeMapCS::PDUCodeMapCS() { construct(); }

void PDUCodeMapCS::construct() {
  addOne(CommunicationCodes::PDU_SiPM0_VSET, 10);
  addOne(CommunicationCodes::PDU_SiPM1_VSET, 11);
  addOne(CommunicationCodes::PDU_SiPM2_VSET, 12);
  addOne(CommunicationCodes::PDU_SiPM3_VSET, 13);
  addOne(CommunicationCodes::PDU_SiPM4_VSET, 14);
  addOne(CommunicationCodes::PDU_SiPM5_VSET, 15);
  addOne(CommunicationCodes::PDU_PressureReg_VSET, 16);
}

} // namespace gramsballoon::pgrams