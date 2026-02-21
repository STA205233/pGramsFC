#ifndef GRAMSBalloon_VCSMapping_hh
#define GRAMSBalloon_VCSMapping_hh 1
#include <cstdint>
#include <optional>
#include <vector>
namespace gramsballoon::pgrams {

/**
 * @brief A virtual class for mapping multiplexer channels to chip select values
 * @author Shota Arai
 * @date 2026-02-21 | Shota Arai | Created
 */
class VCSMapping {
public:
  VCSMapping() = default;
  ~VCSMapping() = default;
  std::optional<uint32_t> getChipSelect(int multiplexerChannel) const;
  void setChipSelect(int multiplexerChannel, uint32_t chipSelect);

protected:
  virtual void registerMapping() = 0;
  void reserveSize(int size) {
    if (size < 0) {
      return;
    }
    csMapping_.reserve(static_cast<size_t>(size));
  }

private:
  std::vector<uint32_t> csMapping_;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_VCSMapping_hh