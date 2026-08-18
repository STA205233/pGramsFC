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
uint16_t crc_calc(const std::vector<uint8_t>& byte_array) {
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

void replace_all(std::string& s,
                 const std::string& from,
                 const std::string& to) {
  if (from.empty()) return;

  std::size_t pos = 0;
  while ((pos = s.find(from, pos)) != std::string::npos) {
    s.replace(pos, from.length(), to);
    pos += to.length();
  }
}

void add_code_map(std::map<std::string, CommandProperty>& code_map,
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

  ADD_CODE_MAP(HUB_TB_Bias_On, 1);
  ADD_CODE_MAP(HUB_TB_Bias_Off, 1);
  ADD_CODE_MAP(HUB_TB_Set_V_Offset, 2);
  ADD_CODE_MAP(HUB_TB_Set_V_Def, 2);
  ADD_CODE_MAP(HUB_TB_Set_Tmux, 2);
  ADD_CODE_MAP(HUB_TB_Query_bias_info, 0);

  ADD_CODE_MAP(PDU_Cold_TPC_HV_ON, 0);
  ADD_CODE_MAP(PDU_Cold_TPC_HV_OFF, 0);
  ADD_CODE_MAP(PDU_Cold_Charge_PreAmp_ON, 0);
  ADD_CODE_MAP(PDU_Cold_Charge_PreAmp_OFF, 0);
  ADD_CODE_MAP(PDU_Cold_SiPM_PreAmp_ON, 0);
  ADD_CODE_MAP(PDU_Cold_SiPM_PreAmp_OFF, 0);
  ADD_CODE_MAP(PDU_Warm_TPC_Shaper_ON, 0);
  ADD_CODE_MAP(PDU_Warm_TPC_Shaper_OFF, 0);
  ADD_CODE_MAP(PDU_SiPM_0_ON, 0);
  ADD_CODE_MAP(PDU_SiPM_0_OFF, 0);
  ADD_CODE_MAP(PDU_SiPM_1_ON, 0);
  ADD_CODE_MAP(PDU_SiPM_1_OFF, 0);
  ADD_CODE_MAP(PDU_SiPM_2_ON, 0);
  ADD_CODE_MAP(PDU_SiPM_2_OFF, 0);
  ADD_CODE_MAP(PDU_SiPM_3_ON, 0);
  ADD_CODE_MAP(PDU_SiPM_3_OFF, 0);
  ADD_CODE_MAP(PDU_SiPM_4_ON, 0);
  ADD_CODE_MAP(PDU_SiPM_4_OFF, 0);
  ADD_CODE_MAP(PDU_SiPM_5_ON, 0);
  ADD_CODE_MAP(PDU_SiPM_5_OFF, 0);
  ADD_CODE_MAP(PDU_Tof_ON, 0);
  ADD_CODE_MAP(PDU_Tof_OFF, 0);
  ADD_CODE_MAP(PDU_TPC_HV_ON, 0);
  ADD_CODE_MAP(PDU_TPC_HV_OFF, 0);
  ADD_CODE_MAP(PDU_CAEN_P3V3_ON, 0);
  ADD_CODE_MAP(PDU_CAEN_P3V3_OFF, 0);
  ADD_CODE_MAP(PDU_CAEN_PM5V_ON, 0);
  ADD_CODE_MAP(PDU_CAEN_PM5V_OFF, 0);
  ADD_CODE_MAP(PDU_CAEN_P12V_ON, 0);
  ADD_CODE_MAP(PDU_CAEN_P12V_OFF, 0);
  ADD_CODE_MAP(PDU_DAQ_CPU_ON, 0);
  ADD_CODE_MAP(PDU_DAQ_CPU_OFF, 0);
  ADD_CODE_MAP(PDU_SiPM0_VSET, 1);
  ADD_CODE_MAP(PDU_SiPM1_VSET, 1);
  ADD_CODE_MAP(PDU_SiPM2_VSET, 1);
  ADD_CODE_MAP(PDU_SiPM3_VSET, 1);
  ADD_CODE_MAP(PDU_SiPM4_VSET, 1);
  ADD_CODE_MAP(PDU_SiPM5_VSET, 1);
  ADD_CODE_MAP(PDU_PressureReg_VSET, 1);

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
  ADD_CODE_MAP(ORC_Boot_Tpc_Daq, 0);
  ADD_CODE_MAP(ORC_Shutdown_Tpc_Daq, 0);
  ADD_CODE_MAP(ORC_Start_PPS, 0);
  ADD_CODE_MAP(ORC_Send_Pulse_Train, 0);
  ADD_CODE_MAP(ORC_Stop_PPS, 0);
  ADD_CODE_MAP(ORC_Restart_Orchestrator, 0);
  ADD_CODE_MAP(ORC_Clear_Errors, 0);
  ADD_CODE_MAP(ORC_Set_Data_SSD0, 0);
  ADD_CODE_MAP(ORC_Set_Data_SSD1, 0);

  ADD_CODE_MAP(TPC_Configure, 1);
  add_code_map(code_map_, "TPC_Configure_File", pgrams::communication::CommunicationCodes::TPC_Configure, 101);
  ADD_CODE_MAP(TPC_Start_Run, 0);
  ADD_CODE_MAP(TPC_Stop_Run, 0);
  ADD_CODE_MAP(TPC_Reset_Run, 0);
  ADD_CODE_MAP(TPC_Boot_DAQ, 0);
  ADD_CODE_MAP(TPC_Boot_Monitor, 0);
  ADD_CODE_MAP(TPCMonitor_Query_LB_Data, 4);
  ADD_CODE_MAP(TPCMonitor_Query_Event_Data, 4);

  ADD_CODE_MAP(TOF_Start_DAQ, 0);
  ADD_CODE_MAP(TOF_Stop_DAQ, 0);
  ADD_CODE_MAP(TOF_Reset_DAQ, 0);
  ADD_CODE_MAP(TOF_Reconnect_Network, 0);

  ADD_CODE_MAP(TOF_Init_System, 0);
  ADD_CODE_MAP(TOF_Make_Bias_Calib_Table, 0);
  ADD_CODE_MAP(TOF_Make_Simple_Bias_Set_Table, 0);
  ADD_CODE_MAP(TOF_Make_Simple_Channel_Map, 0);
  ADD_CODE_MAP(TOF_Make_Simple_Disc_Set_Table, 0);
  ADD_CODE_MAP(TOF_Read_Temperature_Sensors, 0);
  ADD_CODE_MAP(TOF_Acquire_Threshold_Calibration, 0);
  ADD_CODE_MAP(TOF_Acquire_TDC_Calibration, 0);
  ADD_CODE_MAP(TOF_Acquire_QDC_Calibration, 0);
  ADD_CODE_MAP(TOF_Acquire_SiPM_Data, 0);
  ADD_CODE_MAP(TOF_Acquire_Threshold_Calibration_BN, 0);
  ADD_CODE_MAP(TOF_Acquire_Threshold_Calibration_D, 0);
  ADD_CODE_MAP(TOF_Set_FEM_Power_Off, 0);
  ADD_CODE_MAP(TOF_Set_FEM_Power_On, 0);

  ADD_CODE_MAP(TOF_Process_Threshold_Calibration, 0);
  ADD_CODE_MAP(TOF_Process_TDC_Calibration, 0);
  ADD_CODE_MAP(TOF_Process_QDC_Calibration, 0);
  ADD_CODE_MAP(TOF_Convert_Raw_To_Raw, 0);
  ADD_CODE_MAP(TOF_Convert_Raw_To_Singles, 0);

  ADD_CODE_MAP(TOF_Convert_Stg1_To_Stg2, 0);
  ADD_CODE_MAP(TOF_Process_QA_Coin, 0);
  ADD_CODE_MAP(TOF_Process_QA_Iridium, 0);

  ADD_CODE_MAP(TOF_Macro_Thermal_Calib, 0);
  ADD_CODE_MAP(TOF_Macro_Auto_Run_Sequence, 0);
  ADD_CODE_MAP(TOF_Macro_Pre_Bias_Prep, 0);
  ADD_CODE_MAP(TOF_Macro_Post_Bias_Prep, 0);
  ADD_CODE_MAP(TOF_Macro_Cyclic_Run_Loop, 0);
}
#undef ADD_CODE_MAP

CommandProperty CommandBuilder::get_command_property(const std::string& name) const {
  auto command = code_map_.find(name);
  if (command == code_map_.end()) {
    throw CommandException("Invalid command name");
  }

  return command->second;
}

uint16_t CommandBuilder::get_command_code(const std::string& name) const {
  return get_command_property(name).code;
}

int CommandBuilder::get_argnum(const std::string& name) const {
  return get_command_property(name).argnum;
}

std::vector<uint8_t> CommandBuilder::make_byte_array(uint16_t code, const std::vector<int32_t>& arg_array) const {
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

std::vector<uint8_t> CommandBuilder::make_byte_array(const std::string& name, const std::vector<int32_t>& arg_array) const {
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
