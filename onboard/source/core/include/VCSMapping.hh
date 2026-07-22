#ifndef GRAMSBalloon_VCSMapping_hh
#define GRAMSBalloon_VCSMapping_hh 1
#include <cstdint>
#include <map>
#include <optional>
namespace gramsballoon::pgrams {

/**
 * @brief A virtual class for mapping multiplexer channels to chip select values
 * @author Shota Arai
 * @date 2026-02-21 | Shota Arai | Created
 * @date 2026-07-10 | Shota Arai | implementation changed
 */
class VCSMapping {
public:
  using cs_t = uint32_t;
  using pair_t = std::pair<cs_t, cs_t>; // first: bit to be controlled, second: actual value when enabled
  using map_t = std::map<int, pair_t>;
  VCSMapping(cs_t csBitRange = 0x00U, cs_t defaultState = 0x00) : csBitRange_(csBitRange), defaultState_(defaultState) {}
  virtual ~VCSMapping() = default;
  std::optional<pair_t> getChipSelect(int multiplexerChannel) const;
  void setChipSelect(int multiplexerChannel, cs_t chipSelect);
  map_t::const_iterator begin() const { return csMapping_.begin(); }
  map_t::const_iterator end() const { return csMapping_.end(); }
  cs_t CsBitRange() const { return csBitRange_; }
  cs_t DefaultState() const { return defaultState_; }
  int NumChannels() const { return static_cast<int>(csMapping_.size()); }

protected:
  virtual void registerMapping() = 0;

private:
  cs_t csBitRange_;
  map_t csMapping_;
  cs_t defaultState_;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_VCSMapping_hh