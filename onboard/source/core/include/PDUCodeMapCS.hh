#ifndef GB_PDUCodeMapCS_hh
#define GB_PDUCodeMapCS_hh 1
#include "VPDUCodeMap.hh"
#include <cstdint>
namespace gramsballoon::pgrams {

/**
 * @brief Channel Map class for PDU CS
 * @note This is used as singleton.
 * @author Shota Arai
 * @date 2026-07-10 | Shota Arai | First version
 * @date 2026-07-24 | Shota Arai | Inherited from VPDUCodeMap and rename
 */
class PDUCodeMapCS final: public VPDUCodeMap {

public:
  using com_code_t = uint16_t;
  using value_t = uint32_t;
  PDUCodeMapCS();
  ~PDUCodeMapCS() = default;

  PDUCodeMapCS& operator=(const PDUCodeMapCS&) = delete;
  PDUCodeMapCS(PDUCodeMapCS&) = delete;
  static PDUCodeMapCS& getInstance() {
    static PDUCodeMapCS instance;
    return instance;
  }

protected:
  void construct() override;
};
} // namespace gramsballoon::pgrams

#endif // GB_PDUCodeMapCS_hh