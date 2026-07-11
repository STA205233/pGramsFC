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
  VCSMapping() = default;
  virtual ~VCSMapping() = default;
  std::optional<uint32_t> getChipSelect(int multiplexerChannel) const;
  void setChipSelect(int multiplexerChannel, uint32_t chipSelect);
  std::map<int, uint32_t>::const_iterator begin() const { return csMapping_.begin(); }
  std::map<int, uint32_t>::const_iterator end() const { return csMapping_.end(); }

protected:
  virtual void registerMapping() = 0;

private:
  std::map<int, uint32_t> csMapping_;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_VCSMapping_hh