#include "CommunicationCodes.hh"
#ifndef GB_PDUChannelMap_hh
#define GB_PDUChannelMap_hh 1
#include "CommunicationCodes.hh"
#include <map>

namespace gramsballoon::pgrams {

/**
   * @brief Channel Map class for PDU
   * @note This is used as singleton.
   * @author Shota Arai
   * @date 2026-07-10 | Shota Arai | First version
   */
class PDUChannelMap final {

public:
  using com_code_t = uint16_t;
  using cs_msk_t = uint32_t;
  using ch_t = uint32_t;
  PDUChannelMap();
  ~PDUChannelMap() = default;

  PDUChannelMap& operator=(const PDUChannelMap&) = delete;
  PDUChannelMap(PDUChannelMap&) = delete;
  static PDUChannelMap& getInstance() {
    static PDUChannelMap instance;
    return instance;
  }

public:
  bool getMapping(com_code_t code, cs_msk_t& cs) const;

private:
  void construct();
  void addOne(::pgrams::communication::CommunicationCodes code, cs_msk_t cs);
  std::map<com_code_t, cs_msk_t> mapping_;
};
} // namespace gramsballoon::pgrams

#endif //GB_PDUChannelMap_hh