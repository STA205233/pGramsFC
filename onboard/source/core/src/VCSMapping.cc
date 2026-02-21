#include "VCSMapping.hh"
#include <iostream>
namespace gramsballoon::pgrams {
std::optional<uint32_t> VCSMapping::getChipSelect(int multiplexerChannel) const {
  if (multiplexerChannel < 0) {
    return std::nullopt;
  }

  const auto index = static_cast<std::size_t>(multiplexerChannel);
  if (index >= csMapping_.size()) {
    return std::nullopt;
  }

  return csMapping_[index];
}
void VCSMapping::setChipSelect(int multiplexerChannel, uint32_t chipSelect) {
  if (multiplexerChannel < 0) {
    std::cerr << "VCSMapping::setChipSelect error: multiplexerChannel cannot be negative." << std::endl;
    return;
  }

  const auto index = static_cast<size_t>(multiplexerChannel);
  if (index >= csMapping_.size()) {
    csMapping_.resize(index + 1, 0);
  }

  csMapping_[index] = chipSelect;
}
} // namespace gramsballoon::pgrams
