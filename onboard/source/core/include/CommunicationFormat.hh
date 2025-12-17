#ifndef GB_CommunicationFormat_H
#define GB_CommunicationFormat_H 1

#include "CRC16.hh"
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sys/time.h>
#include <vector>
/**
 * @brief A class to define the command format for the pGRAMS system.
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-25
 * @date 2025-03-11 | Modified to fit the pGRAMS command format.
 * @date 2025-04-01 | Name changed from CommandDefinition to CommunicationFormat
 */

namespace gramsballoon::pgrams {

class CommunicationFormat {
public:
  CommunicationFormat();

  bool setData(const std::vector<uint8_t> &v);
  bool setData(const std::string &s);

  template <typename T>
  bool checkHeaderFooter(const T &v);
  template <typename T>
  bool validate(const T &v, uint16_t argc);

  void interpret();
  std::ostream &write(std::ostream &stream);
  template <typename T>
  T getValue(int index);
  template <typename T>
  void getVector(int index, int num, std::vector<T> &vec);

  const std::vector<uint8_t> &Command() const { return command_; }
  std::vector<uint8_t> &CommandNC() { return command_; }
  void CommandStr(std::string &outStr) const { outStr.assign(command_.begin(), command_.end()); }
  uint16_t Code() const { return code_; }
  uint16_t Argc() const { return argc_; }
  const std::vector<int32_t> &Arguments() const { return arguments_; }
  void update();
  void setCode(uint16_t code) {
    code_ = code;
    updated_ = false;
  }
  void setArgc(uint16_t argc) {
    argc_ = argc;
    arguments_.resize(argc_);
    updated_ = false;
  }
  void setArguments(const std::vector<int32_t> &arguments) {
    setArgc(static_cast<uint16_t>(arguments.size()));
    for (uint16_t i = 0; i < argc_; ++i) {
      arguments_[i] = arguments[i];
    }
    updated_ = false;
  }
  void setArguments(uint16_t index, int32_t argument) {
    if (index > argc_) {
      std::cerr << "Error in CommunicationFormat: index(" << index << ") is larger than argc (" << argc_ << ")" << std::endl;
      return;
    }
    arguments_[index] = argument;
    updated_ = false;
  }
  int32_t getArguments(uint16_t index) const {
    if (index < argc_) {
      return arguments_[index];
    }
    return 0;
  }
  void setCommand(const std::vector<uint8_t> &command) { command_ = command; }
  void setCommand(const std::string &command) { command_.assign(command.begin(), command.end()); }
  std::ostream &print(std::ostream &stream) {
    stream << "Code: " << static_cast<int>(code_) << std::endl;
    stream << "Argc: " << static_cast<int>(argc_) << std::endl;
    stream << "Argv: " << std::endl;
    int sz = argc_;
    if (argc_ > 10) {
      sz = 10;
    }
    for (int i = 0; i < sz; i++) {
      stream << static_cast<int>(arguments_[i]) << " ";
    }
    stream << std::endl;
    return stream;
  }

private:
  std::vector<uint8_t> command_;
  uint16_t code_ = 0;
  uint16_t argc_ = 0;
  std::vector<int32_t> arguments_;
  bool updated_ = false;
};
template <typename T>
inline bool CommunicationFormat::checkHeaderFooter(const T &v) {
  const int n = v.size();
  if (n < 10) {
    std::cerr << "Command is too short!!: length = " << n << std::endl;
    return false;
  }

  if (static_cast<uint8_t>(v[0]) != 0xeb || static_cast<uint8_t>(v[1]) != 0x90 || static_cast<uint8_t>(v[2]) != 0x5b || static_cast<uint8_t>(v[3]) != 0x6a) {
    std::cerr << "start code incorect" << std::endl;
    return false;
  }
  if (static_cast<uint8_t>(v[n - 4]) != 0xc5 || static_cast<uint8_t>(v[n - 3]) != 0xa4 || static_cast<uint8_t>(v[n - 2]) != 0xd2 || static_cast<uint8_t>(v[n - 1]) != 0x79) {
    std::cerr << "stop code incorrect" << std::endl;
    return false;
  }
  return true;
}

template <typename T>
bool CommunicationFormat::validate(const T &v, uint16_t argc) {
  const int n = v.size();
  if (n != 14 + 4 * static_cast<int>(argc)) {
    std::cerr << "Invalid command: length not appropriate" << std::endl;
    std::cerr << "The length of command should be " << 14 + 4 * static_cast<int>(argc) << ", but now it is " << n << std::endl;
    return false;
  }

  std::vector<uint8_t> com_without_fotter;
  for (int i = 0; i < n - 6; i++) {
    com_without_fotter.push_back(v[i]);
  }
  uint16_t crc_calc = calcCRC16(com_without_fotter);
  uint16_t crc_attached = getValue<uint16_t>(n - 6);
  if (crc_calc != crc_attached) {
    std::cerr << "Invalid command: CRC16 not appropriate" << std::endl;
    return false;
  }
  return true;
}
} // namespace gramsballoon::pgrams

#endif /* CommandDefinition_H */
