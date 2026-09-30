#include "PDUCodeMapDIO.hh"
#include "CommunicationCodes.hh"

namespace gramsballoon::pgrams {
using ::pgrams::communication::CommunicationCodes;
using value_t = PDUCodeMapDIO::value_t;

PDUCodeMapDIO::PDUCodeMapDIO() { construct(); }

void PDUCodeMapDIO::construct() {
  addOne(CommunicationCodes::PDU_Tof_Bias_OFF, 0);
  addOne(CommunicationCodes::PDU_Tof_Bias_ON, 0);
  addOne(CommunicationCodes::PDU_Tof_ON, 1);
  addOne(CommunicationCodes::PDU_Tof_OFF, 1);
  addOne(CommunicationCodes::PDU_DAQ_CPU_ON, 2);
  addOne(CommunicationCodes::PDU_DAQ_CPU_OFF, 2);
  addOne(CommunicationCodes::PDU_CAEN_PM5V_ON, 3);
  addOne(CommunicationCodes::PDU_CAEN_PM5V_OFF, 3);
  addOne(CommunicationCodes::PDU_CAEN_P12V_ON, 4);
  addOne(CommunicationCodes::PDU_CAEN_P12V_OFF, 4);
  addOne(CommunicationCodes::PDU_CAEN_P3V3_ON, 5);
  addOne(CommunicationCodes::PDU_CAEN_P3V3_OFF, 5);
  addOne(CommunicationCodes::PDU_Warm_TPC_Shaper_ON, 6);
  addOne(CommunicationCodes::PDU_Warm_TPC_Shaper_OFF, 6);
  addOne(CommunicationCodes::PDU_Cold_SiPM_PreAmp_ON, 7);
  addOne(CommunicationCodes::PDU_Cold_SiPM_PreAmp_OFF, 7);
  addOne(CommunicationCodes::PDU_Cold_Charge_PreAmp_ON, 8);
  addOne(CommunicationCodes::PDU_Cold_Charge_PreAmp_OFF, 8);
  addOne(CommunicationCodes::PDU_Cold_TPC_HV_ON, 9);
  addOne(CommunicationCodes::PDU_Cold_TPC_HV_OFF, 9);
  addOne(CommunicationCodes::PDU_SiPM_5_ON, 10);
  addOne(CommunicationCodes::PDU_SiPM_5_OFF, 10);
  addOne(CommunicationCodes::PDU_SiPM_4_ON, 11);
  addOne(CommunicationCodes::PDU_SiPM_4_OFF, 11);
  addOne(CommunicationCodes::PDU_SiPM_3_ON, 12);
  addOne(CommunicationCodes::PDU_SiPM_3_OFF, 12);
  addOne(CommunicationCodes::PDU_SiPM_2_ON, 13);
  addOne(CommunicationCodes::PDU_SiPM_2_OFF, 13);
  addOne(CommunicationCodes::PDU_SiPM_1_ON, 14);
  addOne(CommunicationCodes::PDU_SiPM_1_OFF, 14);
  addOne(CommunicationCodes::PDU_SiPM_0_ON, 15);
  addOne(CommunicationCodes::PDU_SiPM_0_OFF, 15);
}

bool PDUCodeMapDIO::isOnCode(com_code_t code) {
  switch (static_cast<CommunicationCodes>(code)) {
    case CommunicationCodes::PDU_Tof_Bias_ON:
    case CommunicationCodes::PDU_Tof_ON:
    case CommunicationCodes::PDU_DAQ_CPU_ON:
    case CommunicationCodes::PDU_CAEN_PM5V_ON:
    case CommunicationCodes::PDU_CAEN_P12V_ON:
    case CommunicationCodes::PDU_CAEN_P3V3_ON:
    case CommunicationCodes::PDU_Warm_TPC_Shaper_ON:
    case CommunicationCodes::PDU_Cold_SiPM_PreAmp_ON:
    case CommunicationCodes::PDU_Cold_Charge_PreAmp_ON:
    case CommunicationCodes::PDU_Cold_TPC_HV_ON:
    case CommunicationCodes::PDU_SiPM_5_ON:
    case CommunicationCodes::PDU_SiPM_4_ON:
    case CommunicationCodes::PDU_SiPM_3_ON:
    case CommunicationCodes::PDU_SiPM_2_ON:
    case CommunicationCodes::PDU_SiPM_1_ON:
    case CommunicationCodes::PDU_SiPM_0_ON:
      return true;
    default:
      return false;
  }
}

} // namespace gramsballoon::pgrams