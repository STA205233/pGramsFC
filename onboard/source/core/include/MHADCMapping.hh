#ifndef GB_MHADCMapping_hh
#define GB_MHADCMapping_hh 1
#include "HubHKTelemetry.hh"
#include "VHKTelemetryMapping.hh"
#include <memory>
namespace gramsballoon::pgrams {
class MHADCMapping: public VHKTelemetryMapping {

public:
  static constexpr int NUM_CHANNEL_PER_SECTION = 6;
  MHADCMapping() = default;
  virtual ~MHADCMapping() = default;

protected:
  bool getSetter(int index, VHKTelemetryMapping::Setter &setter) const override;

public:
  static constexpr int ChannelMapping(int ADC_index, int channel);
};
} // namespace gramsballoon::pgrams
#endif //GB_MHADCMapping_hh