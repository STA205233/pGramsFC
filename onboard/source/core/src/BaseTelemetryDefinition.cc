#include "BaseTelemetryDefinition.hh"
#include "CommunicationCodes.hh"
#include <iomanip>
#include <sstream>
namespace gramsballoon::pgrams {
BaseTelemetryDefinition::BaseTelemetryDefinition(bool instantiateContents) {
  if (instantiateContents) {
    contents_ = std::make_shared<CommunicationFormat>();
  }
  else {
    contents_ = nullptr;
  }
}
bool parse_packet(const std::string &s,
                  std::string &t, char &stype, int &i, std::string &c) {
  // "t": の位置を探す
  auto tpos = s.find("\"t\":\"");
  auto spos = s.find("\",\"s\":\"", tpos);
  auto ipos = s.find("\",\"i\":", spos);
  auto cpos = s.find(",\"c\":\"", ipos);
  auto endpos = s.rfind("\"}");

  if (tpos == std::string::npos || spos == std::string::npos ||
      ipos == std::string::npos || cpos == std::string::npos ||
      endpos == std::string::npos) {
    std::cout << "BaseTelemetryDefinition:: Format Error" << std::endl;
    return false;
  }
  t = s.substr(tpos + 5, spos - (tpos + 5)); // 10桁
  stype = s[spos + 7]; // 1文字
  i = std::stoi(s.substr(ipos + 6, cpos - (ipos + 6))); // 数値
  c = s.substr(cpos + 6, endpos - (cpos + 6)); // バイナリ部
  return true;
}
void BaseTelemetryDefinition::construct() {
  if (constructed_) {
    return;
  }
  const char *subsystem_str = nullptr;
  switch (subsystem_) {
  case Subsystem::HUB:
    subsystem_str = "h";
    break;
  case Subsystem::ORC:
    subsystem_str = "o";
    break;
  case Subsystem::COL:
    subsystem_str = "c";
    break;
  case Subsystem::QM:
    subsystem_str = "q";
    break;
  case Subsystem::TOF:
    subsystem_str = "t";
    break;
  default:
    subsystem_str = "u";
    break;
  }
  content_.clear();
  if (contents_) {
    contents_->update();
    contents_->CommandStr(content_);
  }
  outss_.str("");
  outss_.clear(std::stringstream::goodbit);
  outss_ << "{\"t\":\"" << getTimeString(timeStamp_) << "\",\"s\":\"" << subsystem_str << "\",\"i\":" << std::setfill('0') << std::setw(6) << index_ << std::setw(0) << std::setfill(' ') << ",\"c\":\"" << content_ << "\"}";
  constructed_ = true;
}
void BaseTelemetryDefinition::construct(std::string &outStr) {
  if (!constructed_) {
    construct();
  }
  outStr = outss_.str();
}
std::string BaseTelemetryDefinition::getTimeString(std::time_t t) {
  char buffer[20];
  std::strftime(buffer, sizeof(buffer), "%m%d%H%M%S", std::localtime(&t));
  return std::string(buffer);
}
void BaseTelemetryDefinition::initializeDBTable(DBFieldSink *sink, const std::string &table_name) const {
  sink->initializeTable(table_name);
  sink->addField("send_time", static_cast<uint64_t>(timeStamp_));
  sink->addField("subsystem", static_cast<uint16_t>(subsystem_));
  sink->addField("telemetry_index", index_);
}
void BaseTelemetryDefinition::serialize(DBFieldSink *sink) const {
  sink->setFieldValue("send_time", static_cast<uint64_t>(timeStamp_));
  sink->setFieldValue("subsystem", static_cast<uint16_t>(subsystem_));
  sink->setFieldValue("telemetry_index", index_);
}
bool BaseTelemetryDefinition::parseJSON(const std::string &jsonString) {
  bool is_success = true;
  constructed_ = false;
  const size_t sz = jsonString.size();
  if (sz < 44) {
    std::cerr << "BaseTelemetryDefinition::parseJSON error: jsonString is too short" << std::endl;
    return false;
  }
  std::string t;
  char stype = 'u';
  int i;
  std::string c;
  if (!parse_packet(jsonString, t, stype, i, c)) {
    std::cerr << "BaseTelemetryDefinition::parseJSON error: regex match failed" << std::endl;
    return false;
  }
  try {
    timeStamp_ = std::stol(t);
  }
  catch (const std::exception &e) {
    std::cerr << "BaseTelemetryDefinition::parseJSON error: time conversion failed: " << e.what() << std::endl;
    is_success = false;
  }
  try {
    if (stype == 'h') {
      subsystem_ = Subsystem::HUB;
    }
    else if (stype == 'o') {
      subsystem_ = Subsystem::ORC;
    }
    else if (stype == 'c') {
      subsystem_ = Subsystem::COL;
    }
    else if (stype == 'q') {
      subsystem_ = Subsystem::QM;
    }
    else if (stype == 't') {
      subsystem_ = Subsystem::TOF;
    }
    else {
      subsystem_ = Subsystem::UNKNOWN;
      is_success = false;
    }
  }
  catch (const std::exception &e) {
    std::cerr << "BaseTelemetryDefinition::parseJSON error: subsystem conversion failed: " << e.what() << std::endl;
    is_success = false;
  }
  try {
    index_ = i;
  }
  catch (const std::exception &e) {
    std::cerr << "BaseTelemetryDefinition::parseJSON error: index conversion failed: " << e.what() << std::endl;
    is_success = false;
  }
  try {
    contents_->setData(c);
    contents_->interpret();
  }
  catch (const std::exception &e) {
    std::cerr << "BaseTelemetryDefinition::parseJSON error: command conversion failed: " << e.what() << std::endl;
    is_success = false;
  }
  interpret();
  return is_success;
}
} // namespace gramsballoon::pgrams