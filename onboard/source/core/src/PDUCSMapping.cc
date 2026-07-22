#include "PDUCSMapping.hh"
#include <cstdint>
namespace gramsballoon::pgrams {
void PDUCSMapping::registerMapping() {
  for (int i = 0; i < 16; ++i) {
    setChipSelect(i, static_cast<uint32_t>(i << 15) | (1 << 16)); // have offset pin 15
  }
  setChipSelect(16, static_cast<uint32_t>(0));
}
} // namespace gramsballoon::pgrams