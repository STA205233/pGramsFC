#include "PDUCommandCSMapping.hh"
namespace gramsballoon::pgrams {
void PDUCommandCSMapping::registerMapping() {
  reserveSize(8);
  setChipSelect(0, 0x01);
  setChipSelect(1, 0x02);
  setChipSelect(2, 0x04);
  setChipSelect(3, 0x08);
  setChipSelect(4, 0x10);
  setChipSelect(5, 0x20);
  setChipSelect(6, 0x40);
  setChipSelect(7, 0x80);
}
} // namespace gramsballoon::pgrams