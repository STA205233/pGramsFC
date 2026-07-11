#include "PDUMapping.hh"

namespace gramsballoon::pgrams {
bool PDUMapping::getSetter(int index, VHKTelemetryMapping::Setter &setter) const {
  switch (index) {
  case ChannelMapping(0, 0):
    setter = &HubHKTelemetry::setPduCurSiPM<0>;
    break;
  case ChannelMapping(0, 1):
    setter = &HubHKTelemetry::setPduVolSiPM<0>;
    break;
  case ChannelMapping(0, 2):
    setter = &HubHKTelemetry::setPduCurSiPM<1>;
    break;
  case ChannelMapping(0, 3):
    setter = &HubHKTelemetry::setPduVolSiPM<1>;
    break;
  case ChannelMapping(0, 4):
    setter = &HubHKTelemetry::setPduCurSiPM<2>;
    break;
  case ChannelMapping(0, 5):
    setter = &HubHKTelemetry::setPduVolSiPM<2>;
    break;
  case ChannelMapping(0, 6):
    setter = &HubHKTelemetry::setPduCurSiPM<3>;
    break;
  case ChannelMapping(0, 7):
    setter = &HubHKTelemetry::setPduVolSiPM<3>;
    break;
  case ChannelMapping(1, 0):
    setter = &HubHKTelemetry::setPduCurSiPM<4>;
    break;
  case ChannelMapping(1, 1):
    setter = &HubHKTelemetry::setPduVolSiPM<4>;
    break;
  case ChannelMapping(1, 2):
    setter = &HubHKTelemetry::setPduCurSiPM<5>;
    break;
  case ChannelMapping(1, 3):
    setter = &HubHKTelemetry::setPduVolSiPM<5>;
    break;
  case ChannelMapping(1, 4):
    setter = &HubHKTelemetry::setPduCurTPCHV;
    break;
  case ChannelMapping(1, 5):
    setter = &HubHKTelemetry::setPduVolTPCHV;
    break;
  case ChannelMapping(1, 6):
    setter = &HubHKTelemetry::setPduHVTemp<0>;
    break;
  case ChannelMapping(1, 7):
    setter = &HubHKTelemetry::setPduHVTemp<1>;
    break;
  case ChannelMapping(2, 0):
    setter = &HubHKTelemetry::setPduCurMainBat;
    break;
  case ChannelMapping(2, 1):
    setter = &HubHKTelemetry::setPduVolMainBat;
    break;
  case ChannelMapping(2, 2):
    setter = &HubHKTelemetry::setPduMainBatTemp;
    break;
  case ChannelMapping(2, 3):
    setter = &HubHKTelemetry::setPduSiPMPreAmpP2V5Cur;
    break;
  case ChannelMapping(2, 4):
    setter = &HubHKTelemetry::setPduSiPMPreAmpP2V5Vol;
    break;
  case ChannelMapping(2, 5):
    setter = &HubHKTelemetry::setPduSiPMPreAmpM5VCur;
    break;
  case ChannelMapping(2, 6):
    setter = &HubHKTelemetry::setPduSiPMPreAmpM5VVol;
    break;
  case ChannelMapping(2, 7):
    setter = &HubHKTelemetry::setPduSiPMPreAmpTemp;
    break;
  case ChannelMapping(3, 0):
    setter = &HubHKTelemetry::setPduChargePreAmpM5VCur;
    break;
  case ChannelMapping(3, 1):
    setter = &HubHKTelemetry::setPduChargePreAmpM5VVol;
    break;
  case ChannelMapping(3, 2):
    setter = &HubHKTelemetry::setPduChargePreAmpP5VCur;
    break;
  case ChannelMapping(3, 3):
    setter = &HubHKTelemetry::setPduChargePreAmpP5VVol;
    break;
  case ChannelMapping(3, 4):
    setter = &HubHKTelemetry::setPduChargePreAmpTemp;
    break;
  case ChannelMapping(3, 5):
    setter = &HubHKTelemetry::setPduTofP12VCur;
    break;
  case ChannelMapping(3, 6):
    setter = &HubHKTelemetry::setPduTofP12VVol;
    break;
  case ChannelMapping(3, 7):
    setter = &HubHKTelemetry::setPduTofP12VTemp;
    break;
  case ChannelMapping(4, 0):
    setter = &HubHKTelemetry::setPduCaenNevisP3V3Cur;
    break;
  case ChannelMapping(4, 1):
    setter = &HubHKTelemetry::setPduCaenNevisP3V3Vol;
    break;
  case ChannelMapping(4, 2):
    setter = &HubHKTelemetry::setPduCaenNevisP5VCur;
    break;
  case ChannelMapping(4, 3):
    setter = &HubHKTelemetry::setPduCaenNevisP5VVol;
    break;
  case ChannelMapping(4, 4):
    setter = &HubHKTelemetry::setPduCaenNevisM5VCur;
    break;
  case ChannelMapping(4, 5):
    setter = &HubHKTelemetry::setPduCaenNevisM5VVol;
    break;
  case ChannelMapping(4, 6):
    setter = &HubHKTelemetry::setPduCaenNevisP12VCur;
    break;
  case ChannelMapping(4, 7):
    setter = &HubHKTelemetry::setPduCaenNevisP12VVol;
    break;
  case ChannelMapping(5, 0):
    setter = &HubHKTelemetry::setPduShaperPCur<0>;
    break;
  case ChannelMapping(5, 1):
    setter = &HubHKTelemetry::setPduShaperMCur<0>;
    break;
  case ChannelMapping(5, 2):
    setter = &HubHKTelemetry::setPduShaperPCur<1>;
    break;
  case ChannelMapping(5, 3):
    setter = &HubHKTelemetry::setPduShaperMCur<1>;
    break;
  case ChannelMapping(5, 4):
    setter = &HubHKTelemetry::setPduShaperPCur<2>;
    break;
  case ChannelMapping(5, 5):
    setter = &HubHKTelemetry::setPduShaperMCur<2>;
    break;
  case ChannelMapping(5, 6):
    setter = &HubHKTelemetry::setPduShaperPCur<3>;
    break;
  case ChannelMapping(5, 7):
    setter = &HubHKTelemetry::setPduShaperMCur<3>;
    break;
  case ChannelMapping(6, 0):
    setter = &HubHKTelemetry::setPduShaperPCur<4>;
    break;
  case ChannelMapping(6, 1):
    setter = &HubHKTelemetry::setPduShaperMCur<4>;
    break;
  case ChannelMapping(6, 2):
    setter = &HubHKTelemetry::setPduShaperPCur<5>;
    break;
  case ChannelMapping(6, 3):
    setter = &HubHKTelemetry::setPduShaperMCur<5>;
    break;
  case ChannelMapping(6, 4):
    setter = &HubHKTelemetry::setPduShaperP3V3Vol;
    break;
  case ChannelMapping(6, 5):
    setter = &HubHKTelemetry::setPduShaperM3V3Vol;
    break;
  case ChannelMapping(6, 6):
    setter = &HubHKTelemetry::setPduShaperTemp;
    break;
  case ChannelMapping(6, 7):
    setter = &HubHKTelemetry::setPduCaenNevisPM5VTemp;
    break;
  case ChannelMapping(7, 0):
    setter = &HubHKTelemetry::setPduCPUCur<0>;
    break;
  case ChannelMapping(7, 1):
    setter = &HubHKTelemetry::setPduCPUCur<1>;
    break;
  case ChannelMapping(7, 2):
    setter = &HubHKTelemetry::setPduCPUCur<2>;
    break;
  case ChannelMapping(7, 3):
    setter = &HubHKTelemetry::setPduCPUCur<3>;
    break;
  case ChannelMapping(7, 4):
    setter = &HubHKTelemetry::setPduCPUCur<4>;
    break;
  case ChannelMapping(7, 5):
    setter = &HubHKTelemetry::setPduCPUCur<5>;
    break;
  case ChannelMapping(7, 6):
    setter = &HubHKTelemetry::setPduCPUCur<6>;
    break;
  case ChannelMapping(7, 7):
    setter = &HubHKTelemetry::setPduCpuUnusedCur;
    break;
  case ChannelMapping(8, 0):
    setter = &HubHKTelemetry::setPduCPUVol<0>;
    break;
  case ChannelMapping(8, 1):
    setter = &HubHKTelemetry::setPduCPUVol<1>;
    break;
  case ChannelMapping(8, 2):
    setter = &HubHKTelemetry::setPduCPUVol<2>;
    break;
  case ChannelMapping(8, 3):
    setter = &HubHKTelemetry::setPduCPUVol<3>;
    break;
  case ChannelMapping(8, 4):
    setter = &HubHKTelemetry::setPduCPUVol<4>;
    break;
  case ChannelMapping(8, 5):
    setter = &HubHKTelemetry::setPduCPUVol<5>;
    break;
  case ChannelMapping(8, 6):
    setter = &HubHKTelemetry::setPduCPUVol<6>;
    break;
  case ChannelMapping(8, 7):
    setter = &HubHKTelemetry::setPressureRegulatorVol;
    break;
  case ChannelMapping(9, 0):
    setter = &HubHKTelemetry::setPduTofBiasP5V0Cur;
    break;
  case ChannelMapping(9, 1):
    setter = &HubHKTelemetry::setPduTofBiasP5V0Vol;
    break;
  case ChannelMapping(9, 2):
    setter = &HubHKTelemetry::setPduTofBiasP5V1Cur;
    break;
  case ChannelMapping(9, 3):
    setter = &HubHKTelemetry::setPduTofBiasP5V1Vol;
    break;
  case ChannelMapping(9, 4):
    setter = &HubHKTelemetry::setPduTofBiasP5VTemp;
    break;
  case ChannelMapping(9, 5):
    setter = &HubHKTelemetry::setPduCaenNevisP12VTemp;
    break;
  case ChannelMapping(9, 6):
    setter = &HubHKTelemetry::setPduCaenNevisP3V3Temp;
    break;
  case ChannelMapping(9, 7):
    setter = &HubHKTelemetry::setPduCommsBoardTemp;
    break;
  default:
    std::cerr << "getSetter: index out of range: " << index << std::endl;
    return false;
  }
  return true;
}
constexpr int PDUMapping::ChannelMapping(int multiplexer_channel, int adc_ch) {
  return NUM_CH_PER_CHIP_SELECT * multiplexer_channel + adc_ch;
}
} // namespace gramsballoon::pgrams