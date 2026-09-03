#include "ToFBiasTelemetry.hh"
#include "BaseTelemetryDefinition.hh"
#include "CommunicationCodes.hh"
#include "DBFieldSink.hh"
#include <array>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
using namespace ::pgrams::communication;
using namespace gramsballoon::pgrams;
namespace {
constexpr std::size_t kChannelsPerMonitor = 8;

bool parseHexField(const std::string &packet, std::size_t pos, std::size_t len,
                   uint32_t &value) {
  if (pos > packet.size() || len > packet.size() - pos) {
    return false;
  }

  const char *begin = packet.data() + pos;
  const char *end = begin + len;
  const auto result = std::from_chars(begin, end, value, 16);
  return result.ec == std::errc{} && result.ptr == end;
}

std::string mode2str(ToFBiasTelemetry::MODE mode) {
  if (mode == ToFBiasTelemetry::MODE::AUTO) {
    return "auto";
  }
  else if (mode == ToFBiasTelemetry::MODE::DEBUG) {
    return "debug";
  }
  return "unknown";
}

} // namespace
namespace gramsballoon::pgrams {
ToFBiasTelemetry::ToFBiasTelemetry(bool instantiateContents) : BaseTelemetryDefinition(instantiateContents) {
  cache_.reserve(0); // we don't use cache_ on the hub computer.
}
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
  cache_.reserve(5000); // this function is used on ground
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

  if (contents->Code() == to_telem_u16(TelemetryCodes::HUB_Tof_Bias_summary)) {
    if (!interpretSummaryOutput()) {
      return false;
    }
  }

  const bool ret = (static_cast<int>(cache_.size()) <= contents->Argc() * 4) && static_cast<int>(cache_.size()) >= (contents->Argc() * 4 - 4);
  return ret;
}

bool ToFBiasTelemetry::interpretSummaryOutput() {
  if (cache_.size() != LENGTH_SUMMARY || cache_[0] != '*' ||
      (cache_[1] != '1' && cache_[1] != '2') ||
      (cache_[4] != '@' && cache_[4] != 'D') ||
      cache_.substr(5, 3) != "xxx" || cache_.substr(536, 2) != "\r\n") {
    return false;
  }

  uint32_t value = 0;
  if (!parseHexField(cache_, 1, 1, value)) {
    return false;
  }
  const uint8_t cardType = static_cast<uint8_t>(value);

  if (!parseHexField(cache_, 2, 2, value)) {
    return false;
  }
  const uint8_t frameCounter = static_cast<uint8_t>(value);
  const MODE mode = cache_[4] == '@' ? MODE::AUTO : (cache_[4] == 'D' ? MODE::DEBUG : MODE::UNKNOWN);

  std::array<uint8_t, 8> hvEnableSetting{};
  for (std::size_t i = 0; i < hvEnableSetting.size(); ++i) {
    if (!parseHexField(cache_, 8 + i, 1, value) || value > 1) {
      return false;
    }
    hvEnableSetting[i] = static_cast<uint8_t>(value);
  }

  std::array<uint16_t, 16> tmonReading{};
  std::array<uint16_t, 16> vmonReading{};
  std::array<uint16_t, 128> trimDACSetting{};
  for (std::size_t monitor = 0; monitor < tmonReading.size(); ++monitor) {
    const std::size_t blockPos = 16 + 32 * monitor;
    if (!parseHexField(cache_, blockPos, 4, value)) {
      return false;
    }
    tmonReading[monitor] = static_cast<uint16_t>(value);

    if (!parseHexField(cache_, blockPos + 4, 4, value)) {
      return false;
    }
    vmonReading[monitor] = static_cast<uint16_t>(value);

    for (std::size_t channel = 0; channel < kChannelsPerMonitor; ++channel) {
      const std::size_t trimIndex = monitor * kChannelsPerMonitor + channel;
      const std::size_t trimPos = blockPos + 8 + 3 * channel;
      if (!parseHexField(cache_, trimPos, 3, value)) {
        return false;
      }
      trimDACSetting[trimIndex] = static_cast<uint16_t>(value);
    }
  }

  std::array<uint16_t, 2> boardTmonReading{};
  for (std::size_t i = 0; i < boardTmonReading.size(); ++i) {
    if (!parseHexField(cache_, 528 + 4 * i, 4, value)) {
      return false;
    }
    boardTmonReading[i] = static_cast<uint16_t>(value);
  }

  cardType_ = cardType;
  frameCounter_ = frameCounter;
  mode_ = mode;
  hvEnableSetting_ = hvEnableSetting;
  tmonReading_ = tmonReading;
  vmonReading_ = vmonReading;
  trimDACSetting_ = trimDACSetting;
  boardTmonReading_ = boardTmonReading;
  return true;
}

void ToFBiasTelemetry::initializeDBTableFull(DBFieldSink *sink, const std::string &table_name) const {

  // For Full output
  BaseTelemetryDefinition::initializeDBTable(sink, table_name);
  sink->addField("full_output", Text());
}
void ToFBiasTelemetry::initializeDBTableSummary(DBFieldSink *sink, const std::string &table_name) const {
  // For Summary
  BaseTelemetryDefinition::initializeDBTable(sink, table_name);
  sink->addField("card_type", static_cast<uint8_t>(0));
  sink->addField("frame_counter", static_cast<uint8_t>(0));
  sink->addField("mode", "");
  for (std::size_t i = 0; i < hvEnableSetting_.size(); ++i) {
    sink->addField("hv_enable_setting_" + std::to_string(i), static_cast<uint8_t>(0));
  }
  for (std::size_t i = 0; i < tmonReading_.size(); ++i) {
    sink->addField("tmon_reading_" + std::to_string(i), static_cast<uint16_t>(0));
    sink->addField("vmon_reading_" + std::to_string(i * kChannelsPerMonitor), static_cast<uint16_t>(0));
  }
  for (std::size_t i = 0; i < trimDACSetting_.size(); ++i) {
    sink->addField("trim_dac_setting_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (std::size_t i = 0; i < boardTmonReading_.size(); ++i) {
    sink->addField("board_tmon_reading_" + std::to_string(i), static_cast<uint16_t>(0));
  }
}

void ToFBiasTelemetry::serialize(DBFieldSink *sink) const {
  if (!getContents() || getContents()->Code() == 0) {
    return;
  }
  BaseTelemetryDefinition::serialize(sink);
  if (getContents()->Code() == ::pgrams::communication::to_telem_u16(::pgrams::communication::TelemetryCodes::HUB_Tof_Bias_full)) {
    sink->setFieldValue("full_output", cache_);
  }
  else if (getContents()->Code() == ::pgrams::communication::to_telem_u16(::pgrams::communication::TelemetryCodes::HUB_Tof_Bias_summary)) {
    sink->setFieldValue("card_type", cardType_);
    sink->setFieldValue("frame_counter", frameCounter_);
    sink->setFieldValue("mode", mode2str(mode_));
    for (std::size_t i = 0; i < hvEnableSetting_.size(); ++i) {
      sink->setFieldValue("hv_enable_setting_" + std::to_string(i), hvEnableSetting_[i]);
    }
    for (std::size_t i = 0; i < tmonReading_.size(); ++i) {
      sink->setFieldValue("tmon_reading_" + std::to_string(i), tmonReading_[i]);
      sink->setFieldValue("vmon_reading_" + std::to_string(i * kChannelsPerMonitor), vmonReading_[i]);
    }
    for (std::size_t i = 0; i < trimDACSetting_.size(); ++i) {
      sink->setFieldValue("trim_dac_setting_" + std::to_string(i), trimDACSetting_[i]);
    }
    for (std::size_t i = 0; i < boardTmonReading_.size(); ++i) {
      sink->setFieldValue("board_tmon_reading_" + std::to_string(i), boardTmonReading_[i]);
    }
  }
}

void ToFBiasTelemetry::reset() {
  BaseTelemetryDefinition::reset();
  cache_.clear();
  cardType_ = 0;
  frameCounter_ = 0;
  mode_ = MODE::UNKNOWN;
  hvEnableSetting_.fill(0);
  tmonReading_.fill(0);
  vmonReading_.fill(0);
  trimDACSetting_.fill(0);
  boardTmonReading_.fill(0);
}
} // namespace gramsballoon::pgrams