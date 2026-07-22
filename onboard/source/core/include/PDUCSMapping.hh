#ifndef GRAMSBalloon_PDUCommandCS_hh
#define GRAMSBalloon_PDUCommandCS_hh 1
#include "VCSMapping.hh"
namespace gramsballoon::pgrams {

/**
 * @brief A class for mapping PDU CS to multiplexer chip select values
 * @author Shota Arai
 * @date 2026-02-21 | Shota Arai | Created
 * @date 2026-07-22 | Shota Arai | Rename
 */
class PDUCSMapping: public VCSMapping {
public:
  PDUCSMapping(cs_t csBitRange = 0x0fff0000) : VCSMapping(csBitRange) {
    registerMapping();
  }
  virtual ~PDUCSMapping() = default;

protected:
  void registerMapping() override;
};
} // namespace gramsballoon::pgrams

#endif // GRAMSBalloon_PDUCommandCS_hh