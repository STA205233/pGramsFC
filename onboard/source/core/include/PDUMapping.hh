#ifndef GRAMSBalloon_PDUMapping_hh
#define GRAMSBalloon_PDUMapping_hh 1
#include "VHKTelemetryMapping.hh"
namespace gramsballoon::pgrams {

/**
 * @brief A class for PDU telemetry order mapping
 * @author Shota Arai
 * @date 2026-02-20 | Shota Arai | Created
 */
class PDUMapping: public VHKTelemetryMapping {
public:
  PDUMapping() = default;
  virtual ~PDUMapping() = default;
  static constexpr int NUM_CH_PER_CHIP_SELECT = 8;

protected:
  bool getSetter(int index, VHKTelemetryMapping::Setter &setter) const override;

public:
  static constexpr int ChannelMapping(int multiplexer_channel, int adc_ch);
};
} // namespace gramsballoon::pgrams

#endif // GRAMSBalloon_PDUMapping_hh