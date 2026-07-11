#include "VCSMapping.hh"
#include <iostream>
namespace gramsballoon::pgrams {
std::optional<uint32_t> VCSMapping::getChipSelect(int multiplexerChannel) const {
  if (csMapping_.count(multiplexerChannel) == 0) {
    return std::nullopt;
  }
  return csMapping_[index];
}
void VCSMapping::setChipSelect(int multiplexerChannel, uint32_t chipSelect) {
  csMapping_.insert_or_assign(multiplexerChannel, chipSelect);
}
} // namespace gramsballoon::pgrams
