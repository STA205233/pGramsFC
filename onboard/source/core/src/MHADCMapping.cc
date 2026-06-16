#include "MHADCMapping.hh"
namespace gramsballoon::pgrams {
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
  case ChannelMapping(0, 6):
    setter = &HubHKTelemetry::setRtdDaqCrate<2>;
    break;
  case ChannelMapping(0, 7):
    setter = &HubHKTelemetry::setRtdShaperFaradayCage<0>;
    break;
  case ChannelMapping(0, 8):
    setter = &HubHKTelemetry::setRtdShaperFaradayCage<1>;
    break;
  case ChannelMapping(0, 9):
    setter = &HubHKTelemetry::setRtdShaperBoard<0>;
    break;
  case ChannelMapping(0, 10):
    setter = &HubHKTelemetry::setRtdShaperBoard<1>;
    break;
  case ChannelMapping(0, 11):
    setter = &HubHKTelemetry::setRtdShaperBoard<2>;
    break;
  case ChannelMapping(0, 12):
    setter = &HubHKTelemetry::setRtdShaperBoard<3>;
    break;
  case ChannelMapping(0, 13):
    setter = &HubHKTelemetry::setRtdShaperBoard<4>;
    break;
  case ChannelMapping(0, 14):
    setter = &HubHKTelemetry::setRtdShaperBoard<5>;
    break;
  case ChannelMapping(0, 15):
    setter = &HubHKTelemetry::setRtdHubComputerLocation<0>;
    break;
  case ChannelMapping(1, 0):
    setter = &HubHKTelemetry::setRtdHubComputerLocation<1>;
    break;
  case ChannelMapping(1, 1):
    setter = &HubHKTelemetry::setRtdTofFpga;
    break;
  case ChannelMapping(1, 2):
    setter = &HubHKTelemetry::setRtdTof;
    break;
  case ChannelMapping(1, 3):
    setter = &HubHKTelemetry::setRtdOutsideSealedEnclosure<0>;
    break;
  case ChannelMapping(1, 4):
    setter = &HubHKTelemetry::setRtdOutsideSealedEnclosure<1>;
    break;
  case ChannelMapping(1, 5):
    setter = &HubHKTelemetry::setRtdVacuumJacket<0>;
    break;
  case ChannelMapping(1, 6):
    setter = &HubHKTelemetry::setRtdVacuumJacket<1>;
    break;
  case ChannelMapping(1, 7):
    setter = &HubHKTelemetry::setRtdVacuumJacket<2>;
    break;
  case ChannelMapping(1, 8):
    setter = &HubHKTelemetry::setSpare<0>;
    break;
  case ChannelMapping(1, 9):
    setter = &HubHKTelemetry::setSpare<1>;
    break;
  case ChannelMapping(1, 10):
    setter = &HubHKTelemetry::setSpare<2>;
    break;
  case ChannelMapping(1, 11):
    setter = &HubHKTelemetry::setSpare<3>;
    break;
  case ChannelMapping(1, 12):
    setter = &HubHKTelemetry::setSpare<4>;
    break;
  case ChannelMapping(1, 13):
    setter = &HubHKTelemetry::setSpare<5>;
    break;
  case ChannelMapping(1, 14):
    setter = &HubHKTelemetry::setSpare<6>;
    break;
  case ChannelMapping(1, 15):
    setter = &HubHKTelemetry::setSpare<7>;
    break;
  case ChannelMapping(2, 0):
    setter = &HubHKTelemetry::setRtdsInsideChamber<0>;
    break;
  case ChannelMapping(2, 1):
    setter = &HubHKTelemetry::setRtdsInsideChamber<1>;
    break;
  case ChannelMapping(2, 2):
    setter = &HubHKTelemetry::setRtdsInsideChamber<2>;
    break;
  case ChannelMapping(2, 3):
    setter = &HubHKTelemetry::setRtdsInsideChamber<3>;
    break;
  case ChannelMapping(2, 4):
    setter = &HubHKTelemetry::setInclinometers<1>;
    break;
  case ChannelMapping(2, 5):
    setter = &HubHKTelemetry::setRtdsInsideChamber<5>;
    break;
  case ChannelMapping(2, 6):
    setter = &HubHKTelemetry::setRtdsInsideChamber<6>;
    break;
  case ChannelMapping(2, 7):
    setter = &HubHKTelemetry::setRtdsInsideChamber<7>;
    break;
  case ChannelMapping(2, 8):
    setter = &HubHKTelemetry::setRtdsInsideChamber<8>;
    break;
  case ChannelMapping(2, 9):
    setter = &HubHKTelemetry::setRtdsInsideChamber<4>;
    break;
  case ChannelMapping(2, 10):
    setter = &HubHKTelemetry::setRtdDaqCrate<0>;
    break;
  case ChannelMapping(2, 11):
    setter = &HubHKTelemetry::setSpare<8>;
    break;
  case ChannelMapping(2, 12):
    setter = &HubHKTelemetry::setSpare<9>;
    break;
  case ChannelMapping(2, 13):
    setter = &HubHKTelemetry::setSpare<10>;
    break;
  case ChannelMapping(2, 14):
    setter = &HubHKTelemetry::setSpare<11>;
    break;
  case ChannelMapping(2, 15):
    setter = &HubHKTelemetry::setSpare<12>;
    break;
  default:
    break;
  }
  return true;
}
} // namespace gramsballoon::pgrams