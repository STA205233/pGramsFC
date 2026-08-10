#include "MHADCMapping.hh"
namespace gramsballoon::pgrams {
constexpr int MHADCMapping::ChannelMapping(int ADC_index, int channel) {
  return NUM_CHANNEL_PER_SECTION * ADC_index + channel;
}
bool MHADCMapping::getSetter(int index, VHKTelemetryMapping::Setter &setter) const {
  switch (index) {
  case ChannelMapping(0, 0):
    setter = &HubHKTelemetry::setRtdGondolaFrame<0>;
    break;
  case ChannelMapping(0, 1):
    setter = &HubHKTelemetry::setRtdGondolaFrame<1>;
    break;
  case ChannelMapping(0, 2):
    setter = &HubHKTelemetry::setRtdGondolaFrame<2>;
    break;
  case ChannelMapping(0, 3):
    setter = &HubHKTelemetry::setRtdGondolaFrame<3>;
    break;
  case ChannelMapping(0, 4):
    setter = &HubHKTelemetry::setInclinometers<0>;
    break;
  case ChannelMapping(0, 5):
    setter = &HubHKTelemetry::setRtdDaqCrate<1>;
    break;
  case ChannelMapping(1, 0):
    setter = &HubHKTelemetry::setRtdDaqCrate<2>;
    break;
  case ChannelMapping(1, 1):
    setter = &HubHKTelemetry::setRtdShaperFaradayCage<0>;
    break;
  case ChannelMapping(1, 2):
    setter = &HubHKTelemetry::setRtdShaperFaradayCage<1>;
    break;
  case ChannelMapping(1, 3):
    setter = &HubHKTelemetry::setRtdShaperBoard<0>;
    break;
  case ChannelMapping(1, 4):
    setter = &HubHKTelemetry::setInclinometers<1>;
    break;
  case ChannelMapping(1, 5):
    setter = &HubHKTelemetry::setRtdShaperBoard<2>;
    break;
  case ChannelMapping(2, 0):
    setter = &HubHKTelemetry::setRtdShaperBoard<1>;
    break;
  case ChannelMapping(2, 1):
    setter = &HubHKTelemetry::setRtdShaperBoard<4>;
    break;
  case ChannelMapping(2, 2):
    setter = &HubHKTelemetry::setRtdShaperBoard<5>;
    break;
  case ChannelMapping(2, 3):
    setter = &HubHKTelemetry::setRtdHubComputerLocation<0>;
    break;
  case ChannelMapping(2, 4):
    setter = &HubHKTelemetry::setRtdHubComputerLocation<1>;
    break;
  case ChannelMapping(2, 5):
    setter = &HubHKTelemetry::setRtdTofFpga;
    break;
  case ChannelMapping(3, 0):
    setter = &HubHKTelemetry::setRtdTof;
    break;
  case ChannelMapping(3, 1):
    setter = &HubHKTelemetry::setRtdOutsideSealedEnclosure<0>;
    break;
  case ChannelMapping(3, 2):
    setter = &HubHKTelemetry::setRtdShaperBoard<3>;
    break;
  case ChannelMapping(3, 3):
    setter = &HubHKTelemetry::setRtdVacuumJacket<0>;
    break;
  case ChannelMapping(3, 4):
    setter = &HubHKTelemetry::setRtdVacuumJacket<1>;
    break;
  case ChannelMapping(3, 5):
    setter = &HubHKTelemetry::setRtdVacuumJacket<2>;
    break;
  case ChannelMapping(4, 0):
    setter = &HubHKTelemetry::setSpare<0>;
    break;
  case ChannelMapping(4, 1):
    setter = &HubHKTelemetry::setSpare<1>;
    break;
  case ChannelMapping(4, 2):
    setter = &HubHKTelemetry::setSpare<2>;
    break;
  case ChannelMapping(4, 3):
    setter = &HubHKTelemetry::setSpare<3>;
    break;
  case ChannelMapping(4, 4):
    setter = &HubHKTelemetry::setSpare<4>;
    break;
  case ChannelMapping(4, 5):
    setter = &HubHKTelemetry::setSpare<5>;
    break;
  case ChannelMapping(5, 0):
    setter = &HubHKTelemetry::setSpare<6>;
    break;
  case ChannelMapping(5, 1):
    setter = &HubHKTelemetry::setSpare<7>;
    break;
  case ChannelMapping(5, 2):
    setter = &HubHKTelemetry::setRtdsInsideChamber<0>;
    break;
  case ChannelMapping(5, 3):
    setter = &HubHKTelemetry::setRtdsInsideChamber<1>;
    break;
  case ChannelMapping(5, 4):
    setter = &HubHKTelemetry::setRtdsInsideChamber<2>;
    break;
  case ChannelMapping(5, 5):
    setter = &HubHKTelemetry::setRtdsInsideChamber<3>;
    break;
  case ChannelMapping(6, 0):
    setter = &HubHKTelemetry::setRtdOutsideSealedEnclosure<0>;
    break;
  case ChannelMapping(6, 1):
    setter = &HubHKTelemetry::setRtdsInsideChamber<5>;
    break;
  case ChannelMapping(6, 2):
    setter = &HubHKTelemetry::setRtdsInsideChamber<6>;
    break;
  case ChannelMapping(6, 3):
    setter = &HubHKTelemetry::setRtdsInsideChamber<7>;
    break;
  case ChannelMapping(6, 4):
    setter = &HubHKTelemetry::setRtdsInsideChamber<8>;
    break;
  case ChannelMapping(6, 5):
    setter = &HubHKTelemetry::setRtdsInsideChamber<4>;
    break;
  case ChannelMapping(7, 0):
    setter = &HubHKTelemetry::setRtdDaqCrate<0>;
    break;
  case ChannelMapping(7, 1):
    setter = &HubHKTelemetry::setSpare<8>;
    break;
  case ChannelMapping(7, 2):
    setter = &HubHKTelemetry::setSpare<9>;
    break;
  case ChannelMapping(7, 3):
    setter = &HubHKTelemetry::setSpare<10>;
    break;
  case ChannelMapping(7, 4):
    setter = &HubHKTelemetry::setSpare<11>;
    break;
  case ChannelMapping(7, 5):
    setter = &HubHKTelemetry::setSpare<12>;
    break;
  default:
    break;
  }
  return true;
}
} // namespace gramsballoon::pgrams