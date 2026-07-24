#ifndef GB_VPDUCodeMap_hh
#define GB_VPDUCodeMap_hh 1
#include "CommunicationCodes.hh"
#include <cstdint>
#include <map>
namespace gramsballoon::pgrams {

/**
 * @brief Virtual Channel Map class for PDU
 * @note This is used as singleton. This class converts CommunicationCodes -> channel value (CS/DIO) value.
 * @author Shota Arai
 * @date 2026-07-24 | Shota Arai | First version
 */
class VPDUCodeMap {

public:
  using com_code_t = uint16_t;
  using value_t = uint32_t;
  VPDUCodeMap() = default;
  ~VPDUCodeMap() = default;

  VPDUCodeMap& operator=(const VPDUCodeMap&) = delete;
  VPDUCodeMap(VPDUCodeMap&) = delete;

public:
  bool getMapping(com_code_t code, value_t& cs) const;

protected:
  virtual void construct() = 0;
  void addOne(::pgrams::communication::CommunicationCodes code, value_t cs);

private:
  std::map<com_code_t, value_t> mapping_;
};
} // namespace gramsballoon::pgrams

#endif // GB_PDUCodeMap_hh