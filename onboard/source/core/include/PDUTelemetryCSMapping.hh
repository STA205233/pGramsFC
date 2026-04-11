#ifndef GRAMSBalloon_PDUTelemetryCS_hh
#define GRAMSBalloon_PDUTelemetryCS_hh 1

#include "VCSMapping.hh"

namespace gramsballoon::pgrams {
/**
 * @brief A class for mapping PDU telemetry to multiplexer chip select values
 * @author Shota Arai
 * @date 2026-02-21 | Shota Arai | Created
 */
class PDUTelemetryCSMapping: public VCSMapping {
public:
  PDUTelemetryCSMapping() : VCSMapping() {
    registerMapping();
  }
  virtual ~PDUTelemetryCSMapping() = default;

protected:
  void registerMapping() override;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_PDUTelemetryCS_hh