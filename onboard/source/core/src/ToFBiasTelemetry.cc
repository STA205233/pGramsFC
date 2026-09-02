#include "ToFBiasTelemetry.hh"
#include "BaseTelemetryDefinition.hh"
#include "CommunicationCodes.hh"
#include "DBFieldSink.hh"
#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
namespace gramsballoon::pgrams {
ToFBiasTelemetry::ToFBiasTelemetry(bool instantiateContents) : BaseTelemetryDefinition(instantiateContents) { cache_.reserve(0); }
std::ostream &ToFBiasTelemetry::print(std::ostream &stream) {
  BaseTelemetryDefinition::print(stream);
  interpret();
  stream << cache_;
  return stream;
}
std::ofstream &ToFBiasTelemetry::write(std::ofstream &stream) {
  BaseTelemetryDefinition::write(stream);
  interpret();
  stream << cache_;
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

bool ToFBiasTelemetry::interpret() {
  cache_.reserve(10000); // this function is used on ground
  auto contents = getContents();
  if (!contents) {
    return false;
  }
  cache_.clear();
  const int sz = contents->Argc();
  for (int i = 0; i < sz; ++i) {
    const uint32_t v = contents->getArguments(i);
    if (!divide(cache_, v)) {
      break;
    }
  }

  const bool ret = (static_cast<int>(cache_.size()) <= contents->Argc() * 4) && static_cast<int>(cache_.size()) >= (contents->Argc() * 4 - 4);
  return ret;
}

void ToFBiasTelemetry::initializeDBTable(DBFieldSink *sink, const std::string &table_name) const {

  // For Full output
  const std::string table_name_for_full_output = table_name + "_full";
  BaseTelemetryDefinition::initializeDBTable(sink, table_name_for_full_output);
  sink->addField("full_output", "");

  // For Summary
  //const std::string table_name_for_summary = table_name + "_summary";
  //BaseTelemetryDefinition::initializeDBTable(sink, table_name_for_summary);
}

void ToFBiasTelemetry::serialize(DBFieldSink *sink) const {
  if (!getContents() || getContents()->Code() == 0) {
    return;
  }
  BaseTelemetryDefinition::serialize(sink);
  if (getContents()->Code() == ::pgrams::communication::to_telem_u16(::pgrams::communication::TelemetryCodes::HUB_Tof_Bias_full)) {
    sink->setFieldValue("full_output", cache_);
  }
}

void ToFBiasTelemetry::reset() {
  BaseTelemetryDefinition::reset();
  cache_.clear();
}
} // namespace gramsballoon::pgrams