#include "CommandDefinition.hh"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <thread>

namespace gramsballoon::pgrams {

CommandDefinition::CommandDefinition() {
  buffer_ = std::make_unique<rapidjson::StringBuffer>();
  writer_ = std::make_unique<rapidjson::Writer<rapidjson::StringBuffer>>(*buffer_);
}

bool CommandDefinition::interpret(const std::string &v) {
  doc_.Parse(v.c_str());
  if (doc_.HasParseError()) {
    std::cerr << "CommandDefinition::interpret error: JSON parse error" << std::endl;
    valid_ = false;
    return false;
  }
  if (!doc_.IsObject()) {
    std::cerr << "CommandDefinition::interpret error: JSON is not object" << std::endl;
    valid_ = false;
    return false;
  }
  if (!doc_.HasMember("code")) {
    std::cerr << "CommandDefinition::interpret error: JSON has no member 'code'" << std::endl;
    valid_ = false;
    return false;
  }
  if (!doc_.HasMember("argc")) {
    std::cerr << "CommandDefinition::interpret error: JSON has no member 'argc'" << std::endl;
    valid_ = false;
    return false;
  }
  const unsigned int argc = doc_["argc"].GetUint();
  if (argc > 0 && !doc_.HasMember("arguments")) {
    std::cerr << "CommandDefinition::interpret error: JSON has no member 'arguments'" << std::endl;
    valid_ = false;
    return false;
  }
  crc_ = static_cast<uint16_t>(doc_["crc"].GetUint());
  code_ = static_cast<uint16_t>(doc_["code"].GetUint());
  argc_ = static_cast<uint16_t>(argc);
  arguments_.clear();
  if (argc_ > 0) {
    const auto &arguments = doc_["arguments"].GetArray();
    for (const auto &v: arguments) {
      arguments_.push_back(v.GetInt());
    }
  }
  makeBytes(true, false);
  valid_ = true;
  return true;
}

bool CommandDefinition::makeDocument() {
  if (!valid_) {
    std::cerr << "CommandDefinition::makeDocument error: invalid command" << std::endl;
    return false;
  }
  doc_.Clear();
  doc_.SetObject();
  doc_.AddMember("code", code_, doc_.GetAllocator());
  doc_.AddMember("argc", argc_, doc_.GetAllocator());
  rapidjson::Value arguments(rapidjson::kArrayType);
  for (const auto &v: arguments_) {
    arguments.PushBack(v, doc_.GetAllocator());
  }
  doc_.AddMember("arguments", arguments, doc_.GetAllocator());
  doc_.AddMember("crc", crc_, doc_.GetAllocator());
  doc_.Accept(*writer_);
  return true;
}

bool CommandDefinition::makeBytes(bool check_crc, bool set_crc) {
  if (argc_ != arguments_.size()) {
    std::cerr << "CommandDefinition::makeBytes error: argc != arguments.size()" << std::endl;
    return false;
  }
  if (!valid_) {
    std::cerr << "CommandDefinition::makeBytes error: invalid command" << std::endl;
    return false;
  }
  command_.clear();
  command_.push_back(0xEB);
  command_.push_back(0x90);
  command_.push_back(0x5B);
  command_.push_back(0x6A);
  command_.push_back(static_cast<uint8_t>((code_ >> 8) & 0xFF));
  command_.push_back(static_cast<uint8_t>(code_ & 0xFF));
  command_.push_back(static_cast<uint8_t>((argc_ >> 8) & 0xFF));
  command_.push_back(static_cast<uint8_t>(argc_ & 0xFF));
  for (const auto &v: arguments_) {
    command_.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
    command_.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
    command_.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    command_.push_back(static_cast<uint8_t>(v & 0xFF));
  }
  if (check_crc || set_crc) {
    const auto crc = calcCRC16(command_);
    if (check_crc && crc_ != crc) {
      std::cerr << "CommandDefinition::makeBytes error: CRC error" << std::endl;
      valid_ = false;
    }
    if (set_crc) {
      crc_ = crc;
    }
  }
  command_.push_back(static_cast<uint8_t>((crc_ >> 8) & 0xFF));
  command_.push_back(static_cast<uint8_t>(crc_ & 0xFF));
  command_.push_back(0xC5);
  command_.push_back(0xA4);
  command_.push_back(0xD2);
  command_.push_back(0x79);
  return valid_;
}

void CommandDefinition::writeFile(const std::string &filename, bool append) {
  std::ofstream ofs;
  if (append) {
    ofs = std::ofstream(filename, std::ios::app | std::ios::binary);
  }
  else {
    ofs = std::ofstream(filename, std::ios::out | std::ios::binary);
  }
  if (!ofs) {
    std::cerr << "File open error." << std::endl;
    return;
  }
  const int size = command_.size();
  ofs.write(reinterpret_cast<char *>(&command_[0]), size);
  ofs.flush();
  ofs.close();
}

} // namespace gramsballoon::pgrams
