#include "PDUMapping.hh"

namespace gramsballoon::pgrams {
bool PDUMapping::getSetter(int index, HubHKSetterResult &setter) {
  switch (index) {
  case 0:
    setSetterArrayResult(&HubHKTelemetry::setPduVolSiPM, 0, setter);
    break;
  case 1:
    setSetterArrayResult(&HubHKTelemetry::setPduVolSiPM, 1, setter);
    break;
  case 2:
    setSetterArrayResult(&HubHKTelemetry::setPduVolSiPM, 2, setter);
    break;
  case 3:
    setSetterArrayResult(&HubHKTelemetry::setPduVolSiPM, 3, setter);
    break;
  case 4:
    setSetterArrayResult(&HubHKTelemetry::setPduVolSiPM, 4, setter);
    break;
  case 5:
    setSetterArrayResult(&HubHKTelemetry::setPduVolSiPM, 5, setter);
    break;
  case 6:
    setSetterArrayResult(&HubHKTelemetry::setPduCurSiPM, 0, setter);
    break;
  case 7:
    setSetterArrayResult(&HubHKTelemetry::setPduCurSiPM, 1, setter);
    break;
  case 8:
    setSetterArrayResult(&HubHKTelemetry::setPduCurSiPM, 2, setter);
    break;
  case 9:
    setSetterArrayResult(&HubHKTelemetry::setPduCurSiPM, 3, setter);
    break;
  case 10:
    setSetterArrayResult(&HubHKTelemetry::setPduCurSiPM, 4, setter);
    break;
  case 11:
    setSetterArrayResult(&HubHKTelemetry::setPduCurSiPM, 5, setter);
    break;
  case 12:
    setSetterResult(&HubHKTelemetry::setPduCurTPCHV, setter);
    break;
  case 13:
    setSetterArrayResult(&HubHKTelemetry::setPduHVTemp, 0, setter);
    break;
  case 14:
    setSetterArrayResult(&HubHKTelemetry::setPduHVTemp, 1, setter);
    break;
  case 15:
    setSetterResult(&HubHKTelemetry::setPduComsBoardTemp, setter);
    break;
  case 16:
    setSetterResult(&HubHKTelemetry::setPduSiPMPreAmpP2V5Vol, setter);
    break;
  case 17:
    setSetterResult(&HubHKTelemetry::setPduSiPMPreAmpP2V5Cur, setter);
    break;
  case 18:
    setSetterResult(&HubHKTelemetry::setPduSiPMPreAmpM5VVol, setter);
    break;
  case 19:
    setSetterResult(&HubHKTelemetry::setPduSiPMPreAmpM5VCur, setter);
    break;
  case 20:
    setSetterResult(&HubHKTelemetry::setPduSiPMPreAmpTemp, setter);
    break;
  case 21:
    setSetterResult(&HubHKTelemetry::setPduChargePreAmpP5VVol, setter);
    break;
  case 22:
    setSetterResult(&HubHKTelemetry::setPduChargePreAmpP5VCur, setter);
    break;
  case 23:
    setSetterResult(&HubHKTelemetry::setPduChargePreAmpM5VVol, setter);
    break;
  case 24:
    setSetterResult(&HubHKTelemetry::setPduChargePreAmpM5VCur, setter);
    break;
  case 25:
    setSetterResult(&HubHKTelemetry::setPduChargePreAmpTemp, setter);
    break;
  case 26:
    setSetterArrayResult(&HubHKTelemetry::setPduToFTelemetry, 0, setter);
    break;
  case 27:
    setSetterArrayResult(&HubHKTelemetry::setPduToFTelemetry, 1, setter);
    break;
  case 28:
    setSetterArrayResult(&HubHKTelemetry::setPduToFTelemetry, 2, setter);
    break;
  case 29:
    setSetterArrayResult(&HubHKTelemetry::setPduToFTelemetry, 3, setter);
    break;
  case 30:
    setSetterArrayResult(&HubHKTelemetry::setPduToFTelemetry, 4, setter);
    break;
  case 31:
    setSetterArrayResult(&HubHKTelemetry::setPduToFTelemetry, 5, setter);
    break;
  case 32:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisP12VVol, setter);
    break;
  case 33:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisP12VCur, setter);
    break;
  case 34:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisM5VVol, setter);
    break;
  case 35:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisM5VCur, setter);
    break;
  case 36:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisP5VCur, setter);
    break;
  case 37:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisP5VVol, setter);
    break;
  case 38:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisP3V3Cur, setter);
    break;
  case 39:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisP3V3Vol, setter);
    break;
  case 40:
    setSetterResult(&HubHKTelemetry::setPduWarmTPCShaperPVol, setter);
    break;
  case 41:
    setSetterResult(&HubHKTelemetry::setPduCaenNevisPM5VTemp, setter);
    break;
  case 42:
    setSetterResult(&HubHKTelemetry::setPduWarmTPCShaperTemp, setter);
    break;
  case 43:
    setSetterResult(&HubHKTelemetry::setPduWarmTPCShaperMVol, setter);
    break;
  case 44:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperPCur, 0, setter);
    break;
  case 45:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperPCur, 1, setter);
    break;
  case 46:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperPCur, 2, setter);
    break;
  case 47:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperPCur, 3, setter);
    break;
  case 48:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperPCur, 4, setter);
    break;
  case 49:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperPCur, 5, setter);
    break;
  case 50:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperMCur, 0, setter);
    break;
  case 51:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperMCur, 1, setter);
    break;
  case 52:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperMCur, 2, setter);
    break;
  case 53:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperMCur, 3, setter);
    break;
  case 54:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperMCur, 4, setter);
    break;
  case 55:
    setSetterArrayResult(&HubHKTelemetry::setPduWarmTPCShaperMCur, 5, setter);
    break;
  case 56:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUCur, 0, setter);
    break;
  case 57:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUCur, 1, setter);
    break;
  case 58:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUCur, 2, setter);
    break;
  case 59:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUCur, 3, setter);
    break;
  case 60:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUCur, 4, setter);
    break;
  case 61:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUCur, 5, setter);
    break;
  case 62:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUCur, 6, setter);
    break;
  case 63:
    setSetterResult(&HubHKTelemetry::setReserved1, setter);
    break;
  case 64:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUVol, 0, setter);
    break;
  case 65:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUVol, 1, setter);
    break;
  case 66:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUVol, 2, setter);
    break;
  case 67:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUVol, 3, setter);
    break;
  case 68:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUVol, 4, setter);
    break;
  case 69:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUVol, 5, setter);
    break;
  case 70:
    setSetterArrayResult(&HubHKTelemetry::setPduCPUVol, 6, setter);
    break;
  case 71:
    setSetterResult(&HubHKTelemetry::setReserved2, setter);
    break;
  default:
    std::cerr << "getSetter: index out of range: " << index << std::endl;
    return false;
  }
  return true;
}
} // namespace gramsballoon::pgrams