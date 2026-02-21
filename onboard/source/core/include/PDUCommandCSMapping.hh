#ifndef GRAMSBalloon_PDUCommandCS_hh
#define GRAMSBalloon_PDUCommandCS_hh 1
#include "VCSMapping.hh"
namespace gramsballoon::pgrams {

/**
 * @brief A class for mapping PDU commands to multiplexer chip select values
 * @author Shota Arai
 * @date 2026-02-21 | Shota Arai | Created
 */
class PDUCommandCSMapping: public VCSMapping {
public:
  PDUCommandCSMapping() = default;
  ~PDUCommandCSMapping() = default;

protected:
  void registerMapping() override;
};
} // namespace gramsballoon::pgrams

#endif // GRAMSBalloon_PDUCommandCS_hh