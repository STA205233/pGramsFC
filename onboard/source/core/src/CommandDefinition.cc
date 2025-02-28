#include "CommandDefinition.hh"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <thread>

namespace gramsballoon {

CommandDefinition::CommandDefinition() {
}

bool CommandDefinition::interpret(const std::string &v) {
  doc_.Parse(v.c_str());
  if (doc_.HasParseError()) {
    std::cerr << "CommandDefinition::interpret error: JSON parse error" << std::endl;
    return false;
  }
  if (!doc_.IsObject()) {
    std::cerr << "CommandDefinition::interpret error: JSON is not object" << std::endl;
    return false;
  }
  if (!doc_.HasMember("code")) {
    std::cerr << "CommandDefinition::interpret error: JSON has no member 'code'" << std::endl;
    return false;
  }
  if (!doc_.HasMember("argc")) {
    std::cerr << "CommandDefinition::interpret error: JSON has no member 'argc'" << std::endl;
    return false;
  }
  const unsigned int argc = doc_["argc"].GetUint();
  if (argc > 0 && !doc_.HasMember("arguments")) {
    std::cerr << "CommandDefinition::interpret error: JSON has no member 'arguments'" << std::endl;
    return false;
  }
  code_ = static_cast<uint16_t>(doc_["code"].GetUInt());
  argc_ = static_cast<uint16_t>(argc);
  arguments_.clear();
  if (argc_ > 0) {
    const auto &arguments = doc_["arguments"].GetArray();
    for (const auto &v: arguments) {
      arguments_.push_back(v.GetInt());
    }
  }
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

template <typename T>
T CommandDefinition::getValue(int index) {
  const int n = command_.size();
  const int byte = sizeof(T);
  if (index + byte > n) {
    std::cerr << "CommandDefinition::getValue error: out of range" << std::endl;
    std::cerr << "command_.size() = " << n << ", index = " << index << ", byte = " << byte << std::endl;
    return static_cast<T>(0);
  }
  if (byte > 8) {
    std::cerr << "CommandDefinition::getValue error: typename error" << std::endl;
    std::cerr << "byte should be equal to or less than 8: byte = " << byte << std::endl;
    return static_cast<T>(0);
  }

  uint32_t v = 0;
  for (int i = 0; i < byte; i++) {
    const int shift = 8 * (byte - 1 - i);
    v |= (static_cast<uint32_t>(command_[index + i]) << (shift));
  }
  return static_cast<T>(v);
}

template <typename T>
void CommandDefinition::getVector(int index, int num, std::vector<T> &vec) {
  const int n = command_.size();
  const int byte = sizeof(T);
  if (index + byte * num > n) {
    std::cerr << "CommandDefinition::getVector error: out of range" << std::endl;
    std::cerr << "command_.size() = " << n << ", index = " << index << ", byte = " << byte
              << ", num = " << num << std::endl;
    return;
  }
  if (byte > 8) {
    std::cerr << "CommandDefinition::getVector error: typename error" << std::endl;
    std::cerr << "byte should be equal to or less than 8: byte = " << byte << std::endl;
    return;
  }
  vec.clear();
  for (int i = 0; i < num; i++) {
    T v = getValue<T>(index);
    vec.push_back(v);
    index += byte;
  }
}

} /* namespace gramsballoon */
