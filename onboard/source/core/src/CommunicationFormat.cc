#include "CommunicationFormat.hh"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <thread>

namespace gramsballoon::pgrams {

CommunicationFormat::CommunicationFormat() {
}

bool CommunicationFormat::setData(const std::vector<uint8_t> &v) {
  if (!checkHeaderFooter(v)) {
    return false;
  }
  command_ = v;
  uint16_t argc = getValue<uint16_t>(6);
  updated_ = true; // We believe the input
  return validate(v, argc);
}

bool CommunicationFormat::setData(const std::string &s) {
  if (!checkHeaderFooter(s)) {
    return false;
  }
  command_.assign(s.begin(), s.end());
  uint16_t argc = getValue<uint16_t>(6);
  updated_ = true; // We believe the input
  return validate(s, argc);
}

void CommunicationFormat::interpret() {
  code_ = getValue<uint16_t>(4);
  argc_ = getValue<uint16_t>(6);

  arguments_.clear();
  getVector<int32_t>(8, static_cast<int>(argc_), arguments_);
  updated_ = false;
}

std::ostream &CommunicationFormat::write(std::ostream &stream) {
  if (!stream) {
    std::cerr << "CommunicationFormat::write error: stream not opened." << std::endl;
    return stream;
  }
  const int size = command_.size();
  stream.write(reinterpret_cast<char *>(&command_[0]), size);
  stream.flush();
  return stream;
}

template <typename T>
T CommunicationFormat::getValue(int index) {
  const int n = command_.size();
  const int byte = sizeof(T);
  if (index + byte > n) {
    std::cerr << "CommunicationFormat::getValue error: out of range" << std::endl;
    std::cerr << "command_.size() = " << n << ", index = " << index << ", byte = " << byte << std::endl;
    return static_cast<T>(0);
  }
  if (byte > 8) {
    std::cerr << "CommunicationFormat::getValue error: typename error" << std::endl;
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
void CommunicationFormat::getVector(int index, int num, std::vector<T> &vec) {
  const int n = command_.size();
  const int byte = sizeof(T);
  if (index + byte * num > n) {
    std::cerr << "CommunicationFormat::getVector error: out of range" << std::endl;
    std::cerr << "command_.size() = " << n << ", index = " << index << ", byte = " << byte
              << ", num = " << num << std::endl;
    return;
  }
  if (byte > 8) {
    std::cerr << "CommunicationFormat::getVector error: typename error" << std::endl;
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
void CommunicationFormat::update() {
  if (updated_) {
    return;
  }
  command_.clear();
  const int argc = arguments_.size();
  argc_ = static_cast<uint16_t>(argc);
  command_.push_back(0xeb);
  command_.push_back(0x90);
  command_.push_back(0x5b);
  command_.push_back(0x6a);
  command_.push_back((code_ >> 8) & 0xff);
  command_.push_back(code_ & 0xff);
  command_.push_back((argc_ >> 8) & 0xff);
  command_.push_back(argc_ & 0xff);
  for (int i = 0; i < argc_; i++) {
    command_.push_back((arguments_[i] >> 24) & 0xff);
    command_.push_back((arguments_[i] >> 16) & 0xff);
    command_.push_back((arguments_[i] >> 8) & 0xff);
    command_.push_back(arguments_[i] & 0xff);
  }
  uint16_t crc = calcCRC16(command_);
  command_.push_back((crc >> 8) & 0xff);
  command_.push_back(crc & 0xff);
  command_.push_back(0xc5);
  command_.push_back(0xa4);
  command_.push_back(0xd2);
  command_.push_back(0x79);
  updated_ = true;
}
} // namespace gramsballoon::pgrams
