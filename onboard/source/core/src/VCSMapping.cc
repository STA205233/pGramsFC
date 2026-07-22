#include "VCSMapping.hh"
#include <utility>
namespace gramsballoon::pgrams {
using cs_t = VCSMapping::cs_t;
using pair_t = VCSMapping::pair_t;
std::optional<pair_t> VCSMapping::getChipSelect(int multiplexerChannel) const {
  if (csMapping_.count(multiplexerChannel) == 0) {
    return std::nullopt;
  }
  return csMapping_.at(multiplexerChannel);
}
void VCSMapping::setChipSelect(int multiplexerChannel, cs_t chipSelect) {
  csMapping_.insert_or_assign(multiplexerChannel, std::make_pair(csBitRange_, chipSelect));
}
} // namespace gramsballoon::pgrams
