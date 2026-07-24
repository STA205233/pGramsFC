#ifndef GB_PDUCodeMapDIO_hh
#define GB_PDUCodeMapDIO_hh 1
#include "VPDUCodeMap.hh"
#include <cstdint>
namespace gramsballoon::pgrams {

/**
 * @brief Channel Map class for PDU DIO
 * @note This is used as singleton.
 * @author Shota Arai
 * @date 2026-07-10 | Shota Arai | First version
 * @date 2026-07-24 | Shota Arai | Inherited from VPDUCodeMap and rename
 */
class PDUCodeMapDIO final: public VPDUCodeMap {

public:
  using com_code_t = uint16_t;
  using value_t = uint32_t;
  PDUCodeMapDIO();
  ~PDUCodeMapDIO() = default;

  PDUCodeMapDIO& operator=(const PDUCodeMapDIO&) = delete;
  PDUCodeMapDIO(PDUCodeMapDIO&) = delete;
  static PDUCodeMapDIO& getInstance() {
    static PDUCodeMapDIO instance;
    return instance;
  }

  static bool isOnCode(com_code_t code);

private:
  void construct() override;
};
} // namespace gramsballoon::pgrams

#endif // GB_PDUChannelMap_hh