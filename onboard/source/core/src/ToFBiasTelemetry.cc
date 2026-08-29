#include "ToFBiasTelemetry.hh"
#include <fstream>
#include <ostream>
namespace gramsballoon::pgrams {
ToFBiasTelemetry::ToFBiasTelemetry(bool instantiateContents) : BaseTelemetryDefinition(instantiateContents) {}
std::ostream &ToFBiasTelemetry::print(std::ostream &stream) {

  return stream;
}
std::ofstream &ToFBiasTelemetry::write(std::ofstream &stream) {
  return stream;
}

void ToFBiasTelemetry::setArguments(const std::string &str) {
  const int sz_str = str.size();
  const int sz = (sz_str + 3) / 4;
  setArgc(sz);
  auto d = str.data();
  for (int i = 0; i < sz; ++i) {
    uint32_t v = 0;
    for (int j = 0; j < 4; ++j) {
      const int index = i * 4 + j;
      uint32_t vv = index < sz_str ? static_cast<uint8_t>(d[index]) : 0;
      v |= (vv << (24 - j * 8));
    }
    BaseTelemetryDefinition::setArguments(i, v);
  }
}

bool divide(std::string &str, uint32_t v) {
  for (int j = 0; j < 4; ++j) {
    const char vv = static_cast<char>((v >> (24 - j * 8)) & 0xff);
    if (vv == 0) {
      return false; // we assume there is no char after 0x00
    }
    str.push_back(vv);
  }
  return true; // continue loop
}

bool ToFBiasTelemetry::toStr(std::string &str) const {
  auto contents = getContents();
  if (!contents) {
    return false;
  }
  str.clear();
  const int sz = contents->Argc();
  for (int i = 0; i < sz; ++i) {
    const uint32_t v = contents->getArguments(i);
    if (!divide(str, v)) {
      break;
    }
  }

  return (str.size() < contents->Argc() * 4) && str.size() >= (contents->Argc() * 4 - 4);
}
} // namespace gramsballoon::pgrams