#include "VHKTelemetryMapping.hh"
namespace gramsballoon::pgrams {
void VHKTelemetryMapping::setValue(int index, uint16_t value) {
  if (!hubHkTelemetry_) {
    std::cerr << "VHKTelemetryMapping: HubHKTelemetry is not set" << std::endl;
    return;
  }
  HubHKSetterResult setter;
  getSetter(index, setter);
  if (setter.setter) {
    (hubHkTelemetry_.get()->*setter.setter)(value);
  }
  else if (setter.setterArray) {
    (hubHkTelemetry_.get()->*setter.setterArray)(setter.indexOfArray, value);
  }
}
} // namespace gramsballoon::pgrams