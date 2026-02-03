#include "CommandBuilder.hh"
#include "CommunicationCodes.hh"
#ifndef SUBSYSTEM_NAME
#define SUBSYSTEM_NAME "DUMMY_SUBSYSTEM"
#endif
#include <algorithm>
#include <cctype>
#include <iostream>
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

void replace_all(std::string &s,
                 const std::string &from,
                 const std::string &to) {
  if (from.empty()) return;

  std::size_t pos = 0;
  while ((pos = s.find(from, pos)) != std::string::npos) {
    s.replace(pos, from.length(), to);
    pos += to.length(); // 無限ループ防止
  }
}

void add_code_map(std::map<std::string, CommandProperty> &code_map,
                  const std::string_view enum_name, const pgrams::communication::CommunicationCodes code,
                  const int argnum) {
  CommandProperty property;
  std::string name = std::string(enum_name);
  if (name.find(std::string(SUBSYSTEM_NAME) + "_") != 0) {
    return;
  }
  replace_all(name, std::string(SUBSYSTEM_NAME) + "_", "");
  property.code = static_cast<uint16_t>(code);
  property.argnum = argnum;
  std::transform(name.begin(), name.end(), name.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  code_map[name] = property;
}

#define ADD_CODE_MAP(name, argnum) \
  add_code_map(code_map_, #name, pgrams::communication::CommunicationCodes::name, argnum);

CommandBuilder::CommandBuilder() {
  ADD_CODE_MAP(HUB_Emergency_Daq_shutdown, 0);
  ADD_CODE_MAP(HUB_Prepare_Shutdown, 0);
  ADD_CODE_MAP(HUB_Exec_Shutdown, 0);
  ADD_CODE_MAP(HUB_Prepare_Restart, 0);
  ADD_CODE_MAP(HUB_Exec_Restart, 0);
  ADD_CODE_MAP(HUB_Reset_Error, 0);
  ADD_CODE_MAP(HUB_Set_Link, 1);
  ADD_CODE_MAP(HUB_Dummy1, 0);
  ADD_CODE_MAP(HUB_Dummy2, 1);

  //ADD_CODE_MAP(PDU_Cold_TPC_HV_ON, 1);
  //ADD_CODE_MAP(PDU_Cold_TPC_HV_OFF, 1);
  //ADD_CODE_MAP(PDU_Cold_Charge_PreAmp_ON, 1);
  //ADD_CODE_MAP(PDU_Cold_Charge_PreAmp_OFF, 1);
  //ADD_CODE_MAP(PDU_Cold_SiPM_PreAmp_ON, 1);
  //ADD_CODE_MAP(PDU_Cold_SiPM_PreAmp_OFF, 1);
  //ADD_CODE_MAP(PDU_Warm_TPC_Shaper_ON, 1);
  //ADD_CODE_MAP(PDU_Warm_TPC_Shaper_OFF, 1);
  //ADD_CODE_MAP(PDU_SiPM_ON, 1);
  //ADD_CODE_MAP(PDU_SiPM_OFF, 1);
  //ADD_CODE_MAP(PDU_CAEN_P3V3_ON, 1);
  //ADD_CODE_MAP(PDU_CAEN_P3V3_OFF, 1);
  //ADD_CODE_MAP(PDU_CAEN_PM5V_ON, 1);
  //ADD_CODE_MAP(PDU_CAEN_PM5V_OFF, 1);
  //ADD_CODE_MAP(PDU_CAEN_P12V_ON, 1);
  //ADD_CODE_MAP(PDU_CAEN_P12V_OFF, 1);
  //ADD_CODE_MAP(PDU_DAQ_CPU_ON, 0);
  //ADD_CODE_MAP(PDU_DAQ_CPU_OFF, 0);
  ADD_CODE_MAP(ORC_Exec_CPU_Restart, 0);
  ADD_CODE_MAP(ORC_Exec_CPU_Shutdown, 0);
  ADD_CODE_MAP(ORC_Boot_All_DAQ, 0);
  ADD_CODE_MAP(ORC_Shutdown_All_DAQ, 0);
  ADD_CODE_MAP(ORC_Start_Computer_Status, 0);
  ADD_CODE_MAP(ORC_Stop_Computer_Status, 0);
  ADD_CODE_MAP(ORC_Init_PCIe_Driver, 0);
  ADD_CODE_MAP(ORC_Boot_Monitor, 0);
  ADD_CODE_MAP(ORC_Shutdown_Monitor, 0);
  ADD_CODE_MAP(ORC_Boot_Tof_Daq, 0);
  ADD_CODE_MAP(ORC_Shutdown_Tof_Daq, 0);

  ADD_CODE_MAP(COL_Configure, 1);
  ADD_CODE_MAP(COL_Start_Run, 0);
  ADD_CODE_MAP(COL_Stop_Run, 0);
  ADD_CODE_MAP(COL_Reset_Run, 0);
  ADD_CODE_MAP(COL_Boot_DAQ, 0);
  ADD_CODE_MAP(COL_Boot_Monitor, 0);
  ADD_CODE_MAP(COL_Query_LB_Data, 0);
  ADD_CODE_MAP(COL_Query_Event_Data, 0);

  ADD_CODE_MAP(TOF_Start_DAQ, 0);
  ADD_CODE_MAP(TOF_Stop_DAQ, 0);
  ADD_CODE_MAP(TOF_Reset_DAQ, 0);
  ADD_CODE_MAP(TOF_Run_Init_System, 0);
  ADD_CODE_MAP(TOF_Run_Make_Bias_Calib_Table, 0);
  ADD_CODE_MAP(TOF_Run_Make_Simple_Bias_Set_Table, 0);
  ADD_CODE_MAP(TOF_Run_Make_Simple_Channel_Map, 0);
  ADD_CODE_MAP(TOF_Run_Make_Simple_Disc_Set_Table, 0);
  ADD_CODE_MAP(TOF_Run_Read_Temperature_Sensors, 0);
  ADD_CODE_MAP(TOF_Run_Acquire_Threshold_Calibration, 0);
  ADD_CODE_MAP(TOF_Run_Acquire_TDC_Calibration, 0);
  ADD_CODE_MAP(TOF_Run_Acquire_QDC_Calibration, 0);
  ADD_CODE_MAP(TOF_Run_Acquire_SiPM_Data, 0);
  ADD_CODE_MAP(TOF_Run_Acquire_Threshold_Calibration_BN, 0);
  ADD_CODE_MAP(TOF_Run_Acquire_Threshold_Calibration_D, 0);
  ADD_CODE_MAP(TOF_Run_Process_Threshold_Calibration, 0);
  ADD_CODE_MAP(TOF_Run_Process_TDC_Calibration, 0);
  ADD_CODE_MAP(TOF_Run_Process_QDC_Calibration, 0);
  ADD_CODE_MAP(TOF_Run_Convert_Raw_To_Raw, 0);
  ADD_CODE_MAP(TOF_Run_Convert_Raw_To_Singles, 0);
  ADD_CODE_MAP(TOF_Run_Convert_Stg1_To_Stg2, 0);
  ADD_CODE_MAP(TOF_Run_Process_Coin_Evt_QA, 0);

  ADD_CODE_MAP(TOF_Bias_ON, 0);
  ADD_CODE_MAP(TOF_Bias_OFF, 0);
  ADD_CODE_MAP(TOF_Bias_Set_Voltage, 1);
}
#undef ADD_CODE_MAP

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
