#include "PDUCommandCSMapping.hh"
namespace gramsballoon::pgrams {
void PDUCommandCSMapping::registerMapping() {
  setChipSelect(0, 0x01);
  setChipSelect(1, 0x02);
  setChipSelect(2, 0x04);
  setChipSelect(3, 0x08);
  setChipSelect(4, 0xf);
  setChipSelect(5, 0x10);
  setChipSelect(6, 0x20);
  setChipSelect(7, 0x40);
  setChipSelect(8, 0x80);
  setChipSelect(9, 0x10);
  setChipSelect(10, 0x20);
  setChipSelect(11, 0x40);
  setChipSelect(12, 0x80);
  setChipSelect(13, 0xf0);
  setChipSelect(14, 0x100);
  setChipSelect(15, 0x200);
}
} // namespace gramsballoon::pgrams