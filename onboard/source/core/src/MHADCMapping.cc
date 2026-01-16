#include "MHADCMapping.hh"
namespace gramsballoon::pgrams {
bool MHADCMapping::getSetter(int index, HubHKSetterResult &setter) {
  switch (index) {
  case 0:
    setSetterArrayResult(&HubHKTelemetry::setRtdGondolaFrame, 0, setter);
    break;
  case 1:
    setSetterArrayResult(&HubHKTelemetry::setRtdGondolaFrame, 1, setter);
    break;
  case 2:
    setSetterArrayResult(&HubHKTelemetry::setRtdGondolaFrame, 2, setter);
    break;
  case 3:
    setSetterArrayResult(&HubHKTelemetry::setRtdGondolaFrame, 3, setter);
    break;
  case 4:
    setSetterResult(&HubHKTelemetry::setRtdDaqCrate1, setter);
    break;
  case 5:
    setSetterResult(&HubHKTelemetry::setRtdDaqCrate2, setter);
    break;
  case 6:
    setSetterResult(&HubHKTelemetry::setRtdDaqCrateBackup, setter);
    break;
  case 7:
    setSetterResult(&HubHKTelemetry::setRtdShaperFaradayCage1, setter);
    break;
  case 8:
    setSetterResult(&HubHKTelemetry::setRtdShaperFaradayCage2, setter);
    break;
  case 9:
    setSetterArrayResult(&HubHKTelemetry::setRtdShaperBoard, 0, setter);
    break;
  case 10:
    setSetterArrayResult(&HubHKTelemetry::setRtdShaperBoard, 1, setter);
    break;
  case 11:
    setSetterArrayResult(&HubHKTelemetry::setRtdShaperBoard, 2, setter);
    break;
  case 12:
    setSetterArrayResult(&HubHKTelemetry::setRtdShaperBoard, 3, setter);
    break;
  case 13:
    setSetterArrayResult(&HubHKTelemetry::setRtdShaperBoard, 4, setter);
    break;
  case 14:
    setSetterArrayResult(&HubHKTelemetry::setRtdShaperBoard, 5, setter);
    break;
  case 15:
    setSetterResult(&HubHKTelemetry::setRtdHubComputerLocation1, setter);
    break;
  case 16:
    setSetterResult(&HubHKTelemetry::setRtdHubComputerLocation2, setter);
    break;
  case 17:
    setSetterResult(&HubHKTelemetry::setRtdTofFpgas, setter);
    break;
  case 18:
    setSetterResult(&HubHKTelemetry::setRtdTof2, setter);
    break;
  case 19:
    setSetterResult(&HubHKTelemetry::setRtdSealedEnclosure1WaterTank, setter);
    break;
  case 20:
    setSetterResult(&HubHKTelemetry::setRtdSealedEnclosureLocation2, setter);
    break;
  case 21:
    setSetterResult(&HubHKTelemetry::setRtdVacuumJacket1, setter);
    break;
  case 22:
    setSetterResult(&HubHKTelemetry::setRtdVacuumJacket2, setter);
    break;
  case 23:
    setSetterResult(&HubHKTelemetry::setRtdVacuumJacket3, setter);
    break;
  case 24:
    setSetterResult(&HubHKTelemetry::setPressureRegulator, setter);
    break;
  case 25:
    setSetterResult(&HubHKTelemetry::setPressureTransducer, setter);
    break;
  case 26:
    setSetterResult(&HubHKTelemetry::setLiquidLevelMeter, setter);
    break;
  case 27:
    setSetterResult(&HubHKTelemetry::setInclinometer, setter);
    break;
  case 28:
    setSetterArrayResult(&HubHKTelemetry::setRtdsInsideChamber, 0, setter);
    break;
  case 29:
    setSetterArrayResult(&HubHKTelemetry::setRtdsInsideChamber, 1, setter);
    break;
  case 30:
    setSetterArrayResult(&HubHKTelemetry::setRtdsInsideChamber, 2, setter);
    break;
  case 31:
    setSetterArrayResult(&HubHKTelemetry::setRtdsInsideChamber, 3, setter);
    break;
  case 32:
    setSetterArrayResult(&HubHKTelemetry::setRtdsInsideChamber, 4, setter);
    break;
  case 33:
    setSetterArrayResult(&HubHKTelemetry::setRtdsInsideChamber, 5, setter);
    break;
  case 34:
    setSetterArrayResult(&HubHKTelemetry::setRtdsInsideChamber, 6, setter);
    break;
  default:
    break;
  }
  return true;
}
} // namespace gramsballoon::pgrams