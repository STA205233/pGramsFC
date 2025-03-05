#ifndef CommandDefinition_H
#define CommandDefinition_H 1

#include "CRC16.hh"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <cstdint>
#include <iostream>
#include <memory>
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
  bool makeDocument();
  bool valid() { return valid_; }
  void writeFile(const std::string &filename, bool append);

  uint16_t Code() { return code_; }
  uint16_t Argc() { return argc_; }
  const std::vector<int32_t> &Arguments() const { return arguments_; }
  const std::vector<uint8_t> &Command() const { return command_; }
  uint16_t CRC() { return crc_; }

  std::string getJsonString() const {
    return buffer_->GetString();
  }

private:
  bool
  makeBytes(bool check_crc, bool set_crc);
  rapidjson::Document doc_;
  std::unique_ptr<rapidjson::StringBuffer> buffer_ = nullptr;
  std::unique_ptr<rapidjson::Writer<rapidjson::StringBuffer>> writer_ = nullptr;
  uint16_t crc_ = 0;
  bool valid_ = false;
  std::vector<uint8_t> command_;
  uint16_t code_ = 0;
  uint16_t argc_ = 0;
  std::vector<int32_t> arguments_;
};

} // namespace gramsballoon::pgrams

#endif /* CommandDefinition_H */
