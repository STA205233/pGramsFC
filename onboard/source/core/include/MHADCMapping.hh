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
  bool getSetter(int index, VHKTelemetryMapping::Setter &setter) const override;

public:
  static constexpr int ChannelMapping(int ADC_index, int channel);
};
inline constexpr int MHADCMapping::ChannelMapping(int ADC_index, int channel) {
  constexpr int NUM_CH_PER_ADC = 16;
  return ADC_index * NUM_CH_PER_ADC + channel;
}
} // namespace gramsballoon::pgrams
#endif //GB_MHADCMapping_hh