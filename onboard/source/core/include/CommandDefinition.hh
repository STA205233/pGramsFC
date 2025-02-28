#ifndef CommandDefinition_H
#define CommandDefinition_H 1

#include "CRC16.hh"
#include "rapidjson/document.h"
#include <cstdint>
#include <iostream>
#include <sys/time.h>
#include <vector>
/**
 * A class to define telemetry
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-25
 * @date 2025-02-28 Shota Arai: Modified for pGRAMS telemetry and command system.
 */

namespace gramsballoon::pgrams {

class CommandDefinition {
public:
  CommandDefinition();

  bool interpret(const std::string &command);
  void writeFile(const std::string &filename, bool append);
  template <typename T>
  T getValue(int index);
  template <typename T>
  void getVector(int index, int num, std::vector<T> &vec);

  uint16_t Code() { return code_; }
  uint16_t Argc() { return argc_; }
  const std::vector<int32_t> &Arguments() const { return arguments_; }

private:
  rapidjson::Document doc_;
  uint16_t code_ = 0;
  uint16_t argc_ = 0;
  std::vector<int32_t> arguments_;
};

} // namespace gramsballoon::pgrams

#endif /* CommandDefinition_H */
