#ifndef GB_VHKTelemetryMapping_hh
#define GB_VHKTelemetryMapping_hh 1
#include "HubHKTelemetry.hh"
#include <memory>
#include <unordered_map>
namespace gramsballoon::pgrams {
/**
 * @brief A virtual class for Telemetry order mapping
 * @author Shota Arai
 * @date 2025-10-22 | Shota Arai | Created
 * @date 2026-06-14 | Shota Arai | Using std::get<> to ensure the range of the array.
 */
class VHKTelemetryMapping {
protected:
  typedef void (HubHKTelemetry::*Setter)(uint16_t);

public:
  VHKTelemetryMapping() = default;
  virtual ~VHKTelemetryMapping() = default;
  void setHKTelemetry(const std::shared_ptr<HubHKTelemetry> &telemetry) {
    hubHkTelemetry_ = telemetry;
  }
  void setValue(int index, uint16_t value);

protected:
  virtual bool getSetter(int index, Setter &setter) const = 0;

private:
  std::shared_ptr<HubHKTelemetry> hubHkTelemetry_ = nullptr;
};
} // namespace gramsballoon::pgrams
#endif //GB_MHADCMapping_hh