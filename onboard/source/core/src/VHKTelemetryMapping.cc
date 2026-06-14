#include "VHKTelemetryMapping.hh"
namespace gramsballoon::pgrams {
void VHKTelemetryMapping::setValue(int index, uint16_t value) {
  if (!hubHkTelemetry_) {
    std::cerr << "VHKTelemetryMapping: HubHKTelemetry is not set" << std::endl;
    return;
  }
  Setter setter;
  getSetter(index, setter);
  if (setter) {
    (hubHkTelemetry_.get()->*setter)(value);
  }
}
} // namespace gramsballoon::pgrams