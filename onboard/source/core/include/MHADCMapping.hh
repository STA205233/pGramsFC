#ifndef GB_MHADCMapping_hh
#define GB_MHADCMapping_hh 1
#include "HubHKTelemetry.hh"
#include "VHKTelemetryMapping.hh"
#include <memory>
namespace gramsballoon::pgrams {
class MHADCMapping: public VHKTelemetryMapping {

public:
  MHADCMapping() = default;
  virtual ~MHADCMapping() = default;

protected:
  bool getSetter(int index, HubHKSetterResult &setter);
};
} // namespace gramsballoon::pgrams
#endif //GB_MHADCMapping_hh