#ifndef GRAMSBALOON_COMMANDBUILDER_H
#define GRAMSBALOON_COMMANDBUILDER_H 1

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace gramsballoon {

struct CommandException {
  CommandException(const std::string &info) { info_ = info; }
  std::string print() const { return info_; }

private:
  std::string info_;
};

struct CommandProperty {
  uint16_t code = 0;
  int argnum = 0;
};

class CommandBuilder {
public:
  CommandBuilder();
  ~CommandBuilder() = default;

  CommandProperty get_command_property(const std::string &name) const;
  uint16_t get_command_code(const std::string &name) const;
  int get_argnum(const std::string &name) const;

  std::vector<uint8_t> make_byte_array(const std::string &name, const std::vector<int32_t> &arg_array) const;
  std::vector<uint8_t> make_byte_array(uint16_t name, const std::vector<int32_t> &arg_array) const;

private:
  std::map<std::string, CommandProperty> code_map_;
};

} /* namespace gramsballoon */

#endif /* GRAMSBALOON_COMMANDBUILDER_H */
