#ifndef GB_VHKTelemetryMapping_hh
#define GB_VHKTelemetryMapping_hh 1
#include "HubHKTelemetry.hh"
#include <memory>
#include <unordered_map>
namespace gramsballoon::pgrams {
class VHKTelemetryMapping {
protected:
  typedef void (HubHKTelemetry::*Setter)(uint16_t);
  typedef void (HubHKTelemetry::*ArraySetter)(int, uint16_t);
  struct HubHKSetterResult {
    Setter setter = nullptr;
    ArraySetter setterArray = nullptr;
    int indexOfArray = 0;
  };

public:
  VHKTelemetryMapping() = default;
  virtual ~VHKTelemetryMapping() = default;
  void setHKTelemetry(const std::shared_ptr<HubHKTelemetry> &telemetry) {
    hubHkTelemetry_ = telemetry;
  }
  void setValue(int index, uint16_t value);

protected:
  virtual bool getSetter(int index, HubHKSetterResult &setter) = 0;
  static void setSetterResult(Setter setter, HubHKSetterResult &result) {
    result.setter = setter;
    result.indexOfArray = 0;
    result.setterArray = nullptr;
  }
  static void setSetterArrayResult(ArraySetter setter, int index, HubHKSetterResult &result) {
    result.setter = nullptr;
    result.setterArray = setter;
    result.indexOfArray = index;
  }

private:
  std::shared_ptr<HubHKTelemetry> hubHkTelemetry_ = nullptr;
};
} // namespace gramsballoon::pgrams
#endif //GB_MHADCMapping_hh