#include "CommandBuilder.hh"
#include <tuple>

namespace {
uint16_t crc_calc(const std::vector<uint8_t> &byte_array) {
  uint16_t crc = 0;
  for (const uint8_t i: byte_array) {
    crc = crc ^ i;
    for (int j = 0; j < 8; j++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0x8408;
      }
      else {
        crc >>= 1;
      }
    }
  }
  return crc;
}

} /* anonymous namespace */

namespace gramsballoon {

CommandBuilder::CommandBuilder() {
  code_map_["get_status"] = CommandProperty{100, 0};
  code_map_["reset_error"] = CommandProperty{101, 0};
  code_map_["exec_shutdown"] = CommandProperty{102, 0};
  code_map_["exec_reboot"] = CommandProperty{103, 0};
  code_map_["prepare_shutdown"] = CommandProperty{104, 0};
  code_map_["prepare_reboot"] = CommandProperty{105, 0};
  code_map_["prepare_software_stop"] = CommandProperty{198, 1};
  code_map_["exec_software_stop"] = CommandProperty{199, 0};
  code_map_["start_detector_readout"] = CommandProperty{201, 0};
  code_map_["stop_detector_readout"] = CommandProperty{202, 0};
  code_map_["set_trigger_mode"] = CommandProperty{203, 1};
  code_map_["set_trigger_channel"] = CommandProperty{204, 2};
  code_map_["set_adc_offset"] = CommandProperty{205, 3};
  code_map_["exec_tpc_hv_output"] = CommandProperty{206, 0};
  code_map_["set_tpc_hv"] = CommandProperty{207, 1};
  code_map_["exec_pmt_hv_output"] = CommandProperty{208, 0};
  code_map_["set_pmt_hv"] = CommandProperty{209, 1};
  code_map_["get_waveform"] = CommandProperty{210, 0};
  code_map_["set_trigger_level"] = CommandProperty{211, 1};
  code_map_["set_trigger_position"] = CommandProperty{212, 1};
  code_map_["set_tpc_hv_upper_limit"] = CommandProperty{301, 1};
  code_map_["set_pmt_hv_upper_limit"] = CommandProperty{302, 1};
  code_map_["dummy_1"] = CommandProperty{900, 0};
  code_map_["dummy_2"] = CommandProperty{901, 1};
  code_map_["dummy_3"] = CommandProperty{902, 0};
  code_map_["dummy_4"] = CommandProperty{903, 5};
  code_map_["invalid_command_1"] = CommandProperty{12345, 0};
  code_map_["Configure"] = CommandProperty{1, 1};
  code_map_["Start_run"] = CommandProperty{1, 0};
  code_map_["Stop_run"] = CommandProperty{1, 0};
}

CommandProperty CommandBuilder::get_command_property(const std::string &name) const {
  auto command = code_map_.find(name);
  if (command == code_map_.end()) {
    throw CommandException("Invalid command name");
  }

  return command->second;
}

uint16_t CommandBuilder::get_command_code(const std::string &name) const {
  return get_command_property(name).code;
}

int CommandBuilder::get_argnum(const std::string &name) const {
  return get_command_property(name).argnum;
}

std::vector<uint8_t> CommandBuilder::make_byte_array(uint16_t code, const std::vector<int32_t> &arg_array) const {
  std::vector<uint8_t> command;
  command.push_back(0xEB);
  command.push_back(0x90);
  command.push_back(0x5B);
  command.push_back(0x6A);

  const int argnum = arg_array.size();
  command.push_back((code & 0xFF00u) >> 8);
  command.push_back((code & 0x00FFu) >> 0);
  command.push_back((argnum & 0xFF00u) >> 8);
  command.push_back((argnum & 0x00FFu) >> 0);

  if (argnum != static_cast<int>(arg_array.size())) {
    throw CommandException("Invalid argument number");
  }

  for (const int32_t arg: arg_array) {
    command.push_back((arg & 0xFF000000u) >> 24);
    command.push_back((arg & 0x00FF0000u) >> 16);
    command.push_back((arg & 0x0000FF00u) >> 8);
    command.push_back((arg & 0x000000FFu) >> 0);
  }

  const uint16_t crc = crc_calc(command);
  command.push_back((crc & 0xFF00u) >> 8);
  command.push_back((crc & 0x00FFu) >> 0);

  // termination word C5A4
  command.push_back(0xC5);
  command.push_back(0xA4);
  command.push_back(0xD2);
  command.push_back(0x79);

  return command;
}

std::vector<uint8_t> CommandBuilder::make_byte_array(const std::string &name, const std::vector<int32_t> &arg_array) const {
  std::vector<uint8_t> command;
  command.push_back(0xEB);
  command.push_back(0x90);
  command.push_back(0x5B);
  command.push_back(0x6A);

  const CommandProperty property = get_command_property(name);
  const uint16_t code = property.code;
  const int argnum = property.argnum;
  command.push_back((code & 0xFF00u) >> 8);
  command.push_back((code & 0x00FFu) >> 0);
  command.push_back((argnum & 0xFF00u) >> 8);
  command.push_back((argnum & 0x00FFu) >> 0);

  if (argnum != static_cast<int>(arg_array.size())) {
    throw CommandException("Invalid argument number");
  }

  for (const int32_t arg: arg_array) {
    command.push_back((arg & 0xFF000000u) >> 24);
    command.push_back((arg & 0x00FF0000u) >> 16);
    command.push_back((arg & 0x0000FF00u) >> 8);
    command.push_back((arg & 0x000000FFu) >> 0);
  }

  const uint16_t crc = crc_calc(command);
  command.push_back((crc & 0xFF00u) >> 8);
  command.push_back((crc & 0x00FFu) >> 0);

  // termination word C5A4
  command.push_back(0xC5);
  command.push_back(0xA4);
  command.push_back(0xD2);
  command.push_back(0x79);

  return command;
}
} /* namespace gramsballoon */
