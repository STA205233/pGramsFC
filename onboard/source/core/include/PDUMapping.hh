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

protected:
  bool getSetter(int index, HubHKSetterResult &setter) override;
};
} // namespace gramsballoon::pgrams

#endif // GRAMSBalloon_PDUMapping_hh