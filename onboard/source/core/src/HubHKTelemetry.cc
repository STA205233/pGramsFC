#include "HubHKTelemetry.hh"
namespace gramsballoon::pgrams {
uint32_t CompileData(uint16_t v1FromMsb, uint16_t v2FromMsb) {
  const uint32_t ret = (v1FromMsb << 16) + v2FromMsb;
  return ret;
}
void DivideData(uint64_t srcValue, uint32_t dest[2]) {
  dest[0] = static_cast<uint32_t>((srcValue >> 32) & 0xFFFFFFFF);
  dest[1] = static_cast<uint32_t>(srcValue & 0xFFFFFFFF);
}
void DivideData(uint32_t srcValue, uint16_t &dest1, uint16_t &dest2) {
  dest1 = static_cast<uint16_t>((srcValue >> 16) & 0xffff);
  dest2 = static_cast<uint16_t>(srcValue & 0xffff);
}

void HubHKTelemetry::serialize(DBFieldSink *sink) const {
  if (!sink) {
    std::cerr << "HubHKTelemetry::serialize: sink is null!" << std::endl;
    return;
  }
  BaseTelemetryDefinition::serialize(sink);
  sink->setFieldValue("run_id", RunID());
  sink->setFieldValue("last_command_code_hub", lastCommandCodeHub_);
  sink->setFieldValue("last_command_index_hub", lastCommandIndexHub_);
  sink->setFieldValue("command_rejected_index_hub", commandRejectedIndexHub_);
  sink->setFieldValue("last_command_code_orc", lastCommandCodeOrc_);
  sink->setFieldValue("last_command_index_orc", lastCommandIndexOrc_);
  sink->setFieldValue("command_rejected_index_orc", commandRejectedIndexOrc_);
  sink->setFieldValue("last_command_code_tpc", lastCommandCodeTPC_);
  sink->setFieldValue("last_command_index_tpc", lastCommandIndexTPC_);
  sink->setFieldValue("command_rejected_index_tpc", commandRejectedIndexTPC_);
  sink->setFieldValue("last_command_code_tof", lastCommandCodeTOF_);
  sink->setFieldValue("last_command_index_tof", lastCommandIndexTOF_);
  sink->setFieldValue("command_rejected_index_tof", commandRejectedIndexTOF_);
  sink->setFieldValue("last_command_code_qm", lastCommandCodeQM_);
  sink->setFieldValue("last_command_index_qm", lastCommandIndexQM_);
  sink->setFieldValue("command_rejected_index_qm", commandRejectedIndexQM_);
  for (size_t i = 0; i < NUM_PDU_SIPM; ++i) {
    sink->setFieldValue("pdu_vol_sipm_" + std::to_string(i), pduVolSiPM_[i]);
    sink->setFieldValue("pdu_cur_sipm_" + std::to_string(i), pduCurSiPM_[i]);
  }

  sink->setFieldValue("pdu_cur_tpc_hv", pduCurTPCHV_);
  for (size_t i = 0; i < NUM_PDU_HV_TEMP; ++i) {
    sink->setFieldValue("pdu_hv_temp_" + std::to_string(i), pduHVTemp_[i]);
  }

  sink->setFieldValue("pdu_coms_board_temp", pduComsBoardTemp_);
  sink->setFieldValue("pdu_sipm_pre_amp_p2v5_vol", pduSiPMPreAmpP2V5Vol_);
  sink->setFieldValue("pdu_sipm_pre_amp_p2v5_cur", pduSiPMPreAmpP2V5Cur_);
  sink->setFieldValue("pdu_sipm_pre_amp_m5v_vol", pduSiPMPreAmpM5VVol_);
  sink->setFieldValue("pdu_sipm_pre_amp_m5v_cur", pduSiPMPreAmpM5VCur_);
  sink->setFieldValue("pdu_sipm_pre_amp_temp", pduSiPMPreAmpTemp_);

  sink->setFieldValue("pdu_charge_pre_amp_p5v_vol", pduChargePreAmpP5VVol_);
  sink->setFieldValue("pdu_charge_pre_amp_p5v_cur", pduChargePreAmpP5VCur_);
  sink->setFieldValue("pdu_charge_pre_amp_m5v_vol", pduChargePreAmpM5VVol_);
  sink->setFieldValue("pdu_charge_pre_amp_m5v_cur", pduChargePreAmpM5VCur_);
  sink->setFieldValue("pdu_charge_pre_amp_temp", pduChargePreAmpTemp_);
  for (size_t i = 0; i < NUM_PDU_TOF_TELEMETRY; ++i) {
    sink->setFieldValue("pdu_tof_telemetry_" + std::to_string(i), pduToFTelemetry_[i]);
  }

  sink->setFieldValue("pdu_caen_nevis_p12v_vol", pduCaenNevisP12VVol_);
  sink->setFieldValue("pdu_caen_nevis_p12v_cur", pduCaenNevisP12VCur_);
  sink->setFieldValue("pdu_caen_nevis_m5v_vol", pduCaenNevisM5VVol_);
  sink->setFieldValue("pdu_caen_nevis_m5v_cur", pduCaenNevisM5VCur_);
  sink->setFieldValue("pdu_caen_nevis_p5v_vol", pduCaenNevisP5VVol_);
  sink->setFieldValue("pdu_caen_nevis_p5v_cur", pduCaenNevisP5VCur_);
  sink->setFieldValue("pdu_caen_nevis_p3v3_vol", pduCaenNevisP3V3Vol_);
  sink->setFieldValue("pdu_caen_nevis_p3v3_cur", pduCaenNevisP3V3Cur_);

  sink->setFieldValue("pdu_warm_tpc_shaper_p_vol", pduWarmTPCShaperPVol_);
  sink->setFieldValue("pdu_caen_nevis_pm5v_temp", pduCaenNevisPM5VTemp_);
  sink->setFieldValue("pdu_warm_tpc_shaper_temp", pduWarmTPCShaperTemp_);
  sink->setFieldValue("pdu_warm_tpc_shaper_m_vol", pduWarmTPCShaperMVol_);

  for (size_t i = 0; i < NUM_PDU_WARM_TPC_SHAPER; ++i) {
    sink->setFieldValue("pdu_warm_tpc_shaper_p_cur_" + std::to_string(i), pduWarmTPCShaperPCur_[i]);
    sink->setFieldValue("pdu_warm_tpc_shaper_m_cur_" + std::to_string(i), pduWarmTPCShaperMCur_[i]);
  }

  for (size_t i = 0; i < NUM_PDU_CPU; ++i) {
    sink->setFieldValue("pdu_cpu_cur_" + std::to_string(i), pduCPUCur_[i]);
    sink->setFieldValue("pdu_cpu_vol_" + std::to_string(i), pduCPUVol_[i]);
  }

  sink->setFieldValue("reserved1", reserved1_);
  sink->setFieldValue("reserved2", reserved2_);
  sink->setFieldValue("pdu_tof_p5v_vol", pduTofP5VVol_);
  sink->setFieldValue("pdu_tof_p5v_cur", pduTofP5VCur_);
  sink->setFieldValue("pdu_tof_temp", pduTofTemp_);
  sink->setFieldValue("pdu_tof_bias_p5v0_vol", pduTofBiasP5V0Vol_);
  sink->setFieldValue("pdu_tof_bias_p5v0_cur", pduTofBiasP5V0Cur_);
  sink->setFieldValue("pdu_tof_bias_temp", pduTofBiasTemp_);
  sink->setFieldValue("pdu_main_dcdc_temp", pduMainDCDCTemp_);

  for (size_t i = 0; i < NUM_RTD_GONDOLA; ++i) {
    sink->setFieldValue("rtd_gondola_frame_" + std::to_string(i), rtdGondolaFrame_[i]);
  }

  sink->setFieldValue("rtd_daq_crate_1", rtdDaqCrate1_);
  sink->setFieldValue("rtd_daq_crate_2", rtdDaqCrate2_);
  sink->setFieldValue("rtd_daq_crate_backup", rtdDaqCrateBackup_);
  sink->setFieldValue("rtd_shaper_faraday_cage_1", rtdShaperFaradayCage1_);
  sink->setFieldValue("rtd_shaper_faraday_cage_2", rtdShaperFaradayCage2_);

  for (size_t i = 0; i < NUM_RTD_SHAPER_BOARD; ++i) {
    sink->setFieldValue("rtd_shaper_board_" + std::to_string(i), rtdShaperBoard_[i]);
  }

  sink->setFieldValue("rtd_hub_computer_location_1", rtdHubComputerLocation1_);
  sink->setFieldValue("rtd_hub_computer_location_2", rtdHubComputerLocation2_);
  sink->setFieldValue("rtd_tof_fpgas", rtdTofFpgas_);
  sink->setFieldValue("rtd_tof_2", rtdTof2_);
  sink->setFieldValue("rtd_sealed_enclosure_1_water_tank", rtdSealedEnclosure1WaterTank_);
  sink->setFieldValue("rtd_sealed_enclosure_location_2", rtdSealedEnclosureLocation2_);
  sink->setFieldValue("rtd_vacuum_jacket_1", rtdVacuumJacket1_);
  sink->setFieldValue("rtd_vacuum_jacket_2", rtdVacuumJacket2_);
  sink->setFieldValue("rtd_vacuum_jacket_3", rtdVacuumJacket3_);

  sink->setFieldValue("pressure_regulator", pressureRegulator_);
  sink->setFieldValue("pressure_transducer", pressureTransducer_);
  sink->setFieldValue("liquid_level_meter", liquidLevelMeter_);
  sink->setFieldValue("inclinometer", inclinometer_);

  for (size_t i = 0; i < NUM_RTD_IN_CHAMBER; ++i) {
    sink->setFieldValue("rtd_inside_chamber_" + std::to_string(i), rtdsInsideChamber_[i]);
  }

  for (size_t i = 0; i < NUM_SPARE; ++i) {
    sink->setFieldValue("spare_" + std::to_string(i), spare_[i]);
  }

  for (size_t i = 0; i < NUM_ERROR_FLAGS; ++i) {
    sink->setFieldValue("error_flag_" + std::to_string(i), hubComputerErrorFlags_[i]);
  }

  sink->setFieldValue("storage_size", storageSize_);
  sink->setFieldValue("cpu_temperature", cpuTemperature_);
  sink->setFieldValue("ram_usage", ramUsage_);
}
void HubHKTelemetry::initializeDBTable(DBFieldSink *sink, const std::string &table_name) const {
  BaseTelemetryDefinition::initializeDBTable(sink, table_name);
  sink->addField("run_id", static_cast<uint32_t>(0));
  sink->addField("last_command_code_hub", static_cast<uint32_t>(0));
  sink->addField("last_command_index_hub", static_cast<uint16_t>(0));
  sink->addField("rejected_command_index_hub", static_cast<uint32_t>(0));
  sink->addField("last_command_code_orc", static_cast<uint32_t>(0));
  sink->addField("last_command_index_orc", static_cast<uint16_t>(0));
  sink->addField("rejected_command_index_orc", static_cast<uint32_t>(0));
  sink->addField("last_command_code_tpc", static_cast<uint32_t>(0));
  sink->addField("last_command_index_tpc", static_cast<uint16_t>(0));
  sink->addField("rejected_command_index_tpc", static_cast<uint32_t>(0));
  sink->addField("last_command_code_tof", static_cast<uint32_t>(0));
  sink->addField("last_command_index_tof", static_cast<uint16_t>(0));
  sink->addField("rejected_command_index_tof", static_cast<uint32_t>(0));
  sink->addField("last_command_code_qm", static_cast<uint32_t>(0));
  sink->addField("last_command_index_qm", static_cast<uint16_t>(0));
  sink->addField("rejected_command_index_qm", static_cast<uint32_t>(0));
  for (size_t i = 0; i < NUM_PDU_SIPM; i++) {
    sink->addField("pdu_vol_sipm_" + std::to_string(i), static_cast<uint16_t>(0));
    sink->addField("pdu_cur_sipm_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("pdu_cur_tpc_hv", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_PDU_HV_TEMP; i++) {
    sink->addField("pdu_hv_temp_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("pdu_coms_board_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_pre_amp_p2v5_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_pre_amp_p2v5_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_pre_amp_m5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_pre_amp_m5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_pre_amp_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_pre_amp_p5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_pre_amp_p5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_pre_amp_m5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_pre_amp_m5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_pre_amp_temp", static_cast<uint16_t>(0));
  for (size_t i = 0; i < 6; i++) {
    sink->addField("pdu_tof_telemetry_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("pdu_caen_nevis_p12v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p12v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_m5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_m5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p3v3_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p3v3_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_warm_tpc_shaper_p_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_pm5v_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_warm_tpc_shaper_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_warm_tpc_shaper_m_vol", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_PDU_WARM_TPC_SHAPER; i++) {
    sink->addField("pdu_warm_tpc_shaper_p_cur_" + std::to_string(i), static_cast<uint16_t>(0));
    sink->addField("pdu_warm_tpc_shaper_m_cur_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_PDU_CPU; i++) {
    sink->addField("pdu_cpu_cur_" + std::to_string(i), static_cast<uint16_t>(0));
    sink->addField("pdu_cpu_vol_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("reserved1", static_cast<uint16_t>(0));
  sink->addField("reserved2", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_p5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_p5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_bias_p5v0_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_bias_p5v0_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_bias_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_main_dcdc_temp", static_cast<uint16_t>(0));

  for (size_t i = 0; i < NUM_RTD_GONDOLA; i++) {
    sink->addField("rtd_gondola_frame_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("rtd_daq_crate_1", static_cast<uint16_t>(0));
  sink->addField("rtd_daq_crate_2", static_cast<uint16_t>(0));
  sink->addField("rtd_daq_crate_backup", static_cast<uint16_t>(0));
  sink->addField("rtd_shaper_faraday_cage_1", static_cast<uint16_t>(0));
  sink->addField("rtd_shaper_faraday_cage_2", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_RTD_SHAPER_BOARD; i++) {
    sink->addField("rtd_shaper_board_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("rtd_hub_computer_location_1", static_cast<uint16_t>(0));
  sink->addField("rtd_hub_computer_location_2", static_cast<uint16_t>(0));
  sink->addField("rtd_tof_fpgas", static_cast<uint16_t>(0));
  sink->addField("rtd_tof_2", static_cast<uint16_t>(0));
  sink->addField("rtd_sealed_enclosure_1_water_tank", static_cast<uint16_t>(0));
  sink->addField("rtd_sealed_enclosure_location_2", static_cast<uint16_t>(0));
  sink->addField("rtd_vacuum_jacket_1", static_cast<uint16_t>(0));
  sink->addField("rtd_vacuum_jacket_2", static_cast<uint16_t>(0));
  sink->addField("rtd_vacuum_jacket_3", static_cast<uint16_t>(0));
  sink->addField("pressure_regulator", static_cast<uint16_t>(0));
  sink->addField("pressure_transducer", static_cast<uint16_t>(0));
  sink->addField("liquid_level_meter", static_cast<uint16_t>(0));
  sink->addField("inclinometer", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_RTD_IN_CHAMBER; i++) {
    sink->addField("rtd_inside_chamber_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < 11; i++) {
    sink->addField("spare_" + std::to_string(i), static_cast<uint16_t>(0));
  }

  for (size_t i = 0; i < NUM_ERROR_FLAGS; i++) {
    sink->addField("error_flag_" + std::to_string(i), static_cast<uint64_t>(0));
  }

  sink->addField("storage_size", static_cast<uint32_t>(0));
  sink->addField("cpu_temperature", static_cast<uint16_t>(0));
  sink->addField("ram_usage", static_cast<uint16_t>(0));
}
bool HubHKTelemetry::interpret() {
  auto contents = getContents();
  if (!contents) {
    return false;
  }
  if (contents->Argc() != ARGC) {
    std::cerr << "HubHKTelemetry::interpret(): Argc() != " << ARGC << std::endl;
    return false;
  }
  setRunID(static_cast<uint32_t>(contents->getArguments(0)));
  lastCommandIndexHub_ = static_cast<uint32_t>(contents->getArguments(1));
  lastCommandIndexOrc_ = static_cast<uint32_t>(contents->getArguments(2));
  lastCommandIndexTPC_ = static_cast<uint32_t>(contents->getArguments(3));
  lastCommandIndexTOF_ = static_cast<uint32_t>(contents->getArguments(4));
  lastCommandIndexQM_ = static_cast<uint32_t>(contents->getArguments(5));
  DivideData(static_cast<uint32_t>(contents->getArguments(6)), lastCommandCodeHub_, lastCommandCodeOrc_);
  DivideData(static_cast<uint32_t>(contents->getArguments(7)), lastCommandCodeTPC_, lastCommandCodeTOF_);
  DivideData(static_cast<uint32_t>(contents->getArguments(8)), lastCommandCodeQM_, pduVolSiPM_[0]);
  DivideData(static_cast<uint32_t>(contents->getArguments(9)), pduVolSiPM_[1], pduVolSiPM_[2]);
  DivideData(static_cast<uint32_t>(contents->getArguments(10)), pduVolSiPM_[3], pduVolSiPM_[4]);
  DivideData(static_cast<uint32_t>(contents->getArguments(11)), pduVolSiPM_[5], pduCurSiPM_[0]);
  DivideData(static_cast<uint32_t>(contents->getArguments(12)), pduCurSiPM_[1], pduCurSiPM_[2]);
  DivideData(static_cast<uint32_t>(contents->getArguments(13)), pduCurSiPM_[3], pduCurSiPM_[4]);
  DivideData(static_cast<uint32_t>(contents->getArguments(14)), pduCurSiPM_[5], pduCurTPCHV_);
  DivideData(static_cast<uint32_t>(contents->getArguments(15)), pduHVTemp_[0], pduHVTemp_[1]);
  DivideData(static_cast<uint32_t>(contents->getArguments(16)), pduComsBoardTemp_, pduSiPMPreAmpP2V5Vol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(17)), pduSiPMPreAmpP2V5Cur_, pduSiPMPreAmpM5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(18)), pduSiPMPreAmpM5VCur_, pduSiPMPreAmpTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(19)), pduChargePreAmpP5VVol_, pduChargePreAmpP5VCur_);
  DivideData(static_cast<uint32_t>(contents->getArguments(20)), pduChargePreAmpM5VVol_, pduChargePreAmpM5VCur_);
  DivideData(static_cast<uint32_t>(contents->getArguments(21)), pduChargePreAmpTemp_, pduToFTelemetry_[0]);
  DivideData(static_cast<uint32_t>(contents->getArguments(22)), pduToFTelemetry_[1], pduToFTelemetry_[2]);
  DivideData(static_cast<uint32_t>(contents->getArguments(23)), pduToFTelemetry_[3], pduToFTelemetry_[4]);
  DivideData(static_cast<uint32_t>(contents->getArguments(24)), pduToFTelemetry_[5], pduCaenNevisP12VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(25)), pduCaenNevisP12VCur_, pduCaenNevisM5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(26)), pduCaenNevisM5VCur_, pduCaenNevisP5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(27)), pduCaenNevisP5VCur_, pduCaenNevisP3V3Vol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(28)), pduCaenNevisP3V3Cur_, pduWarmTPCShaperPVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(29)), pduCaenNevisPM5VTemp_, pduWarmTPCShaperTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(30)), pduWarmTPCShaperMVol_, pduWarmTPCShaperPCur_[0]);
  DivideData(static_cast<uint32_t>(contents->getArguments(31)), pduWarmTPCShaperPCur_[1], pduWarmTPCShaperPCur_[2]);
  DivideData(static_cast<uint32_t>(contents->getArguments(32)), pduWarmTPCShaperPCur_[3], pduWarmTPCShaperPCur_[4]);
  DivideData(static_cast<uint32_t>(contents->getArguments(33)), pduWarmTPCShaperPCur_[5], pduWarmTPCShaperMCur_[0]);
  DivideData(static_cast<uint32_t>(contents->getArguments(34)), pduWarmTPCShaperMCur_[1], pduWarmTPCShaperMCur_[2]);
  DivideData(static_cast<uint32_t>(contents->getArguments(35)), pduWarmTPCShaperMCur_[3], pduWarmTPCShaperMCur_[4]);
  DivideData(static_cast<uint32_t>(contents->getArguments(36)), pduWarmTPCShaperMCur_[5], pduCPUVol_[0]);
  DivideData(static_cast<uint32_t>(contents->getArguments(37)), pduCPUVol_[1], pduCPUVol_[2]);
  DivideData(static_cast<uint32_t>(contents->getArguments(38)), pduCPUVol_[3], pduCPUVol_[4]);
  DivideData(static_cast<uint32_t>(contents->getArguments(39)), pduCPUVol_[5], pduCPUVol_[6]);
  DivideData(static_cast<uint32_t>(contents->getArguments(40)), pduCPUCur_[0], pduCPUCur_[1]);
  DivideData(static_cast<uint32_t>(contents->getArguments(41)), pduCPUCur_[2], pduCPUCur_[3]);
  DivideData(static_cast<uint32_t>(contents->getArguments(42)), pduCPUCur_[4], pduCPUCur_[5]);
  DivideData(static_cast<uint32_t>(contents->getArguments(43)), pduCPUCur_[6], reserved1_);
  DivideData(static_cast<uint32_t>(contents->getArguments(44)), reserved2_, pduTofP5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(45)), pduTofP5VCur_, pduTofTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(46)), pduTofBiasP5V0Vol_, pduTofBiasP5V0Cur_);
  DivideData(static_cast<uint32_t>(contents->getArguments(47)), pduTofBiasTemp_, pduMainDCDCTemp_);
  // MHADC
  DivideData(static_cast<uint32_t>(contents->getArguments(48)), rtdGondolaFrame_[0], rtdGondolaFrame_[1]);
  DivideData(static_cast<uint32_t>(contents->getArguments(49)), rtdGondolaFrame_[2], rtdGondolaFrame_[3]);
  DivideData(static_cast<uint32_t>(contents->getArguments(50)), rtdDaqCrate1_, rtdDaqCrate2_);
  DivideData(static_cast<uint32_t>(contents->getArguments(51)), rtdDaqCrateBackup_, rtdShaperFaradayCage1_);
  DivideData(static_cast<uint32_t>(contents->getArguments(52)), rtdShaperFaradayCage2_, rtdShaperBoard_[0]);
  DivideData(static_cast<uint32_t>(contents->getArguments(53)), rtdShaperBoard_[1], rtdShaperBoard_[2]);
  DivideData(static_cast<uint32_t>(contents->getArguments(54)), rtdShaperBoard_[3], rtdShaperBoard_[4]);
  DivideData(static_cast<uint32_t>(contents->getArguments(55)), rtdShaperBoard_[5], rtdHubComputerLocation1_);
  DivideData(static_cast<uint32_t>(contents->getArguments(56)), rtdHubComputerLocation2_, rtdTofFpgas_);
  DivideData(static_cast<uint32_t>(contents->getArguments(57)), rtdTof2_, rtdSealedEnclosure1WaterTank_);
  DivideData(static_cast<uint32_t>(contents->getArguments(58)), rtdSealedEnclosureLocation2_, rtdVacuumJacket1_);
  DivideData(static_cast<uint32_t>(contents->getArguments(59)), rtdVacuumJacket2_, rtdVacuumJacket3_);
  DivideData(static_cast<uint32_t>(contents->getArguments(60)), pressureRegulator_, pressureTransducer_);
  DivideData(static_cast<uint32_t>(contents->getArguments(61)), liquidLevelMeter_, inclinometer_);
  DivideData(static_cast<uint32_t>(contents->getArguments(62)), rtdsInsideChamber_[0], rtdsInsideChamber_[1]);
  DivideData(static_cast<uint32_t>(contents->getArguments(63)), rtdsInsideChamber_[2], rtdsInsideChamber_[3]);
  DivideData(static_cast<uint32_t>(contents->getArguments(64)), rtdsInsideChamber_[4], rtdsInsideChamber_[5]);
  DivideData(static_cast<uint32_t>(contents->getArguments(65)), rtdsInsideChamber_[6], spare_[0]);
  DivideData(static_cast<uint32_t>(contents->getArguments(66)), spare_[1], spare_[2]);
  DivideData(static_cast<uint32_t>(contents->getArguments(67)), spare_[3], spare_[4]);
  DivideData(static_cast<uint32_t>(contents->getArguments(68)), spare_[5], spare_[6]);
  DivideData(static_cast<uint32_t>(contents->getArguments(69)), spare_[7], spare_[8]);
  DivideData(static_cast<uint32_t>(contents->getArguments(70)), spare_[9], spare_[10]);

  for (size_t i = 0; i < NUM_TOF_BIAS / 2; i++) {
    DivideData(static_cast<uint32_t>(contents->getArguments(71 + i)), tofBiasVoltage_[2 * i], tofBiasVoltage_[2 * i + 1]);
  }
  for (size_t i = 0; i < NUM_ERROR_FLAGS; i++) {
    hubComputerErrorFlags_[i] = static_cast<uint32_t>(contents->getArguments(71 + NUM_TOF_BIAS / 2 + i));
  }
  storageSize_ = contents->getArguments(71 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS);
  DivideData(static_cast<uint32_t>(contents->getArguments(72 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS)), cpuTemperature_, ramUsage_);
  commandRejectedIndexHub_ = contents->getArguments(73 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS);
  commandRejectedIndexOrc_ = contents->getArguments(74 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS);
  commandRejectedIndexTPC_ = contents->getArguments(75 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS);
  commandRejectedIndexTOF_ = contents->getArguments(76 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS);
  commandRejectedIndexQM_ = contents->getArguments(77 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS);
  return true;
}
void HubHKTelemetry::update() {
  auto contents = getContents();
  if (!contents) {
    setContents(std::make_shared<CommunicationFormat>());
  }
  setArgc(ARGC);
  setArguments(0, RunID());
  setArguments(1, lastCommandIndexHub_);
  setArguments(2, lastCommandIndexOrc_);
  setArguments(3, lastCommandIndexTPC_);
  setArguments(4, lastCommandIndexTOF_);
  setArguments(5, lastCommandIndexQM_);
  setArguments(6, CompileData(lastCommandCodeHub_, lastCommandCodeOrc_));
  setArguments(7, CompileData(lastCommandCodeTPC_, lastCommandCodeTOF_));
  setArguments(8, CompileData(lastCommandCodeQM_, pduVolSiPM_[0]));
  setArguments(9, CompileData(pduVolSiPM_[1], pduVolSiPM_[2]));
  setArguments(10, CompileData(pduVolSiPM_[3], pduVolSiPM_[4]));
  setArguments(11, CompileData(pduVolSiPM_[5], pduCurSiPM_[0]));
  setArguments(12, CompileData(pduCurSiPM_[1], pduCurSiPM_[2]));
  setArguments(13, CompileData(pduCurSiPM_[3], pduCurSiPM_[4]));
  setArguments(14, CompileData(pduCurSiPM_[5], pduCurTPCHV_));
  setArguments(15, CompileData(pduHVTemp_[0], pduHVTemp_[1]));
  setArguments(16, CompileData(pduComsBoardTemp_, pduSiPMPreAmpP2V5Vol_));
  setArguments(17, CompileData(pduSiPMPreAmpP2V5Cur_, pduSiPMPreAmpM5VVol_));
  setArguments(18, CompileData(pduSiPMPreAmpM5VCur_, pduSiPMPreAmpTemp_));
  setArguments(19, CompileData(pduChargePreAmpP5VVol_, pduChargePreAmpP5VCur_));
  setArguments(20, CompileData(pduChargePreAmpM5VVol_, pduChargePreAmpM5VCur_));
  setArguments(21, CompileData(pduChargePreAmpTemp_, pduToFTelemetry_[0]));
  setArguments(22, CompileData(pduToFTelemetry_[1], pduToFTelemetry_[2]));
  setArguments(23, CompileData(pduToFTelemetry_[3], pduToFTelemetry_[4]));
  setArguments(24, CompileData(pduToFTelemetry_[5], pduCaenNevisP12VVol_));
  setArguments(25, CompileData(pduCaenNevisP12VCur_, pduCaenNevisM5VVol_));
  setArguments(26, CompileData(pduCaenNevisM5VCur_, pduCaenNevisP5VVol_));
  setArguments(27, CompileData(pduCaenNevisP5VCur_, pduCaenNevisP3V3Vol_));
  setArguments(28, CompileData(pduCaenNevisP3V3Cur_, pduWarmTPCShaperPVol_));
  setArguments(29, CompileData(pduCaenNevisPM5VTemp_, pduWarmTPCShaperTemp_));
  setArguments(30, CompileData(pduWarmTPCShaperMVol_, pduWarmTPCShaperPCur_[0]));
  setArguments(31, CompileData(pduWarmTPCShaperPCur_[1], pduWarmTPCShaperPCur_[2]));
  setArguments(32, CompileData(pduWarmTPCShaperPCur_[3], pduWarmTPCShaperPCur_[4]));
  setArguments(33, CompileData(pduWarmTPCShaperPCur_[5], pduWarmTPCShaperMCur_[0]));
  setArguments(34, CompileData(pduWarmTPCShaperMCur_[1], pduWarmTPCShaperMCur_[2]));
  setArguments(35, CompileData(pduWarmTPCShaperMCur_[3], pduWarmTPCShaperMCur_[4]));
  setArguments(36, CompileData(pduWarmTPCShaperMCur_[5], pduCPUVol_[0]));
  setArguments(37, CompileData(pduCPUVol_[1], pduCPUVol_[2]));
  setArguments(38, CompileData(pduCPUVol_[3], pduCPUVol_[4]));
  setArguments(39, CompileData(pduCPUVol_[5], pduCPUVol_[6]));
  setArguments(40, CompileData(pduCPUCur_[0], pduCPUCur_[1]));
  setArguments(41, CompileData(pduCPUCur_[2], pduCPUCur_[3]));
  setArguments(42, CompileData(pduCPUCur_[4], pduCPUCur_[5]));
  setArguments(43, CompileData(pduCPUCur_[6], reserved1_));
  setArguments(44, CompileData(reserved2_, pduTofP5VVol_));
  setArguments(45, CompileData(pduTofP5VCur_, pduTofTemp_));
  setArguments(46, CompileData(pduTofBiasP5V0Vol_, pduTofBiasP5V0Cur_));
  setArguments(47, CompileData(pduTofBiasTemp_, pduMainDCDCTemp_));

  // MHADC
  setArguments(48, CompileData(rtdGondolaFrame_[0], rtdGondolaFrame_[1]));
  setArguments(49, CompileData(rtdGondolaFrame_[2], rtdGondolaFrame_[3]));
  setArguments(50, CompileData(rtdDaqCrate1_, rtdDaqCrate2_));
  setArguments(51, CompileData(rtdDaqCrateBackup_, rtdShaperFaradayCage1_));
  setArguments(52, CompileData(rtdShaperFaradayCage2_, rtdShaperBoard_[0]));
  setArguments(53, CompileData(rtdShaperBoard_[1], rtdShaperBoard_[2]));
  setArguments(54, CompileData(rtdShaperBoard_[3], rtdShaperBoard_[4]));
  setArguments(55, CompileData(rtdShaperBoard_[5], rtdHubComputerLocation1_));
  setArguments(56, CompileData(rtdHubComputerLocation2_, rtdTofFpgas_));
  setArguments(57, CompileData(rtdTof2_, rtdSealedEnclosure1WaterTank_));
  setArguments(58, CompileData(rtdSealedEnclosureLocation2_, rtdVacuumJacket1_));
  setArguments(59, CompileData(rtdVacuumJacket2_, rtdVacuumJacket3_));
  setArguments(60, CompileData(pressureRegulator_, pressureTransducer_));
  setArguments(61, CompileData(liquidLevelMeter_, inclinometer_));
  setArguments(62, CompileData(rtdsInsideChamber_[0], rtdsInsideChamber_[1]));
  setArguments(63, CompileData(rtdsInsideChamber_[2], rtdsInsideChamber_[3]));
  setArguments(64, CompileData(rtdsInsideChamber_[4], rtdsInsideChamber_[5]));
  setArguments(65, CompileData(rtdsInsideChamber_[6], spare_[0]));
  setArguments(66, CompileData(spare_[1], spare_[2]));
  setArguments(67, CompileData(spare_[3], spare_[4]));
  setArguments(68, CompileData(spare_[5], spare_[6]));
  setArguments(69, CompileData(spare_[7], spare_[8]));
  setArguments(70, CompileData(spare_[9], spare_[10]));
  static_assert(NUM_TOF_BIAS % 2 == 0, "NUM_TOF_BIAS is expected to be even.");
  for (size_t i = 0; i < NUM_TOF_BIAS / 2; i++) {
    setArguments(71 + i, CompileData(tofBiasVoltage_[2 * i], tofBiasVoltage_[2 * i + 1]));
  }
  for (size_t i = 0; i < NUM_ERROR_FLAGS; i++) {
    setArguments(71 + NUM_TOF_BIAS / 2 + i, hubComputerErrorFlags_[i]);
  }
  setArguments(71 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS, storageSize_);
  setArguments(72 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS, CompileData(cpuTemperature_, ramUsage_));
  setArguments(73 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS, commandRejectedIndexHub_);
  setArguments(74 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS, commandRejectedIndexOrc_);
  setArguments(75 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS, commandRejectedIndexTPC_);
  setArguments(76 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS, commandRejectedIndexTOF_);
  setArguments(77 + NUM_TOF_BIAS / 2 + NUM_ERROR_FLAGS, commandRejectedIndexQM_);
  BaseTelemetryDefinition::update();
}
std::ostream &HubHKTelemetry::print(std::ostream &stream) {
  stream << "HubHKTelemetry" << std::endl;
  stream << "index: " << getContents()->Code() << ", Argc(): " << getContents()->Argc() << std::endl;
  stream << "Data: " << std::endl;
  BaseTelemetryDefinition::print(stream);
  stream << "RunID_: " << RunID() << "\n";
  stream << "lastCommandIndexHub_: " << lastCommandIndexHub_ << ", lastCommandIndexOrc_: " << lastCommandIndexOrc_
         << ", lastCommandIndexTPC_: " << lastCommandIndexTPC_ << ", lastCommandIndexTOF_: " << lastCommandIndexTOF_
         << ", lastCommandIndexQM_: " << lastCommandIndexQM_ << "\n";
  stream << "lastCommandCodeHub_: " << lastCommandCodeHub_ << ", lastCommandCodeOrc_: " << lastCommandCodeOrc_
         << ", lastCommandCodeTPC_: " << lastCommandCodeTPC_ << ", lastCommandCodeTOF_: " << lastCommandCodeTOF_
         << ", lastCommandCodeQM_: " << lastCommandCodeQM_ << "\n";
  stream << "commandRejectedIndexHub_: " << commandRejectedIndexHub_ << ", commandRejectedIndexOrc_: " << commandRejectedIndexOrc_
         << ", commandRejectedIndexTPC_: " << commandRejectedIndexTPC_ << ", commandRejectedIndexTOF_: " << commandRejectedIndexTOF_
         << ", commandRejectedIndexQM_: " << commandRejectedIndexQM_ << "\n";

  {
    const size_t n = sizeof(pduVolSiPM_) / sizeof(pduVolSiPM_[0]);
    stream << "pduVolSiPM_: ";
    for (size_t i = 0; i < n; ++i)
      stream << pduVolSiPM_[i] << (i + 1 < n ? ", " : "\n");
  }

  {
    const size_t n = sizeof(pduCurSiPM_) / sizeof(pduCurSiPM_[0]);
    stream << "pduCurSiPM_: ";
    for (size_t i = 0; i < n; ++i)
      stream << pduCurSiPM_[i] << (i + 1 < n ? ", " : "\n");
  }
  stream << "pduCurTPCHV_: " << pduCurTPCHV_ << "\n";

  stream << "pduHVTemp_: " << pduHVTemp_[0] << ", " << pduHVTemp_[1] << "\n";
  stream << "pduComsBoardTemp_: " << pduComsBoardTemp_ << ", pduSiPMPreAmpP2V5Vol_: " << pduSiPMPreAmpP2V5Vol_ << "\n";
  stream << "pduSiPMPreAmpP2V5Cur_: " << pduSiPMPreAmpP2V5Cur_ << ", pduSiPMPreAmpM5VVol_: " << pduSiPMPreAmpM5VVol_ << "\n";
  stream << "pduSiPMPreAmpM5VCur_: " << pduSiPMPreAmpM5VCur_ << ", pduSiPMPreAmpTemp_: " << pduSiPMPreAmpTemp_ << "\n";

  stream << "pduChargePreAmpP5VVol_: " << pduChargePreAmpP5VVol_ << ", pduChargePreAmpP5VCur_: " << pduChargePreAmpP5VCur_ << "\n";
  stream << "pduChargePreAmpM5VVol_: " << pduChargePreAmpM5VVol_ << ", pduChargePreAmpM5VCur_: " << pduChargePreAmpM5VCur_ << "\n";
  stream << "pduChargePreAmpTemp_: " << pduChargePreAmpTemp_ << "\n";

  {
    const size_t n = sizeof(pduToFTelemetry_) / sizeof(pduToFTelemetry_[0]);
    stream << "pduToFTelemetry_: ";
    for (size_t i = 0; i < n; ++i)
      stream << pduToFTelemetry_[i] << (i + 1 < n ? ", " : "\n");
  }

  stream << "pduCaenNevisP12VVol_: " << pduCaenNevisP12VVol_ << ", pduCaenNevisP12VCur_: " << pduCaenNevisP12VCur_ << "\n";
  stream << "pduCaenNevisM5VVol_: " << pduCaenNevisM5VVol_ << ", pduCaenNevisM5VCur_: " << pduCaenNevisM5VCur_ << "\n";
  stream << "pduCaenNevisP5VVol_: " << pduCaenNevisP5VVol_ << ", pduCaenNevisP5VCur_: " << pduCaenNevisP5VCur_ << "\n";
  stream << "pduCaenNevisP3V3Vol_: " << pduCaenNevisP3V3Vol_ << ", pduCaenNevisP3V3Cur_: " << pduCaenNevisP3V3Cur_ << "\n";

  stream << "pduWarmTPCShaperPVol_: " << pduWarmTPCShaperPVol_ << ", pduWarmTPCShaperTemp_: " << pduWarmTPCShaperTemp_ << "\n";
  stream << "pduWarmTPCShaperMVol_: " << pduWarmTPCShaperMVol_ << "\n";

  {
    const size_t n = sizeof(pduWarmTPCShaperPCur_) / sizeof(pduWarmTPCShaperPCur_[0]);
    stream << "pduWarmTPCShaperPCur_: ";
    for (size_t i = 0; i < n; ++i)
      stream << pduWarmTPCShaperPCur_[i] << (i + 1 < n ? ", " : "\n");
  }
  {
    const size_t n = sizeof(pduWarmTPCShaperMCur_) / sizeof(pduWarmTPCShaperMCur_[0]);
    stream << "pduWarmTPCShaperMCur_: ";
    for (size_t i = 0; i < n; ++i)
      stream << pduWarmTPCShaperMCur_[i] << (i + 1 < n ? ", " : "\n");
  }

  {
    const size_t n = sizeof(pduCPUVol_) / sizeof(pduCPUVol_[0]);
    stream << "pduCPUVol_: ";
    for (size_t i = 0; i < n; ++i)
      stream << pduCPUVol_[i] << (i + 1 < n ? ", " : "\n");
  }
  {
    const size_t n = sizeof(pduCPUCur_) / sizeof(pduCPUCur_[0]);
    stream << "pduCPUCur_: ";
    for (size_t i = 0; i < n; ++i)
      stream << pduCPUCur_[i] << (i + 1 < n ? ", " : "\n");
  }

  stream << "reserved1_: " << reserved1_ << ", reserved2_: " << reserved2_ << "\n";
  stream << "pduTofP5VVol_: " << pduTofP5VVol_ << ", pduTofP5VCur_: " << pduTofP5VCur_ << ", pduTofTemp_: " << pduTofTemp_ << "\n";

  stream << "pduTofBiasP5V0Vol_: " << pduTofBiasP5V0Vol_ << ", pduTofBiasP5V0Cur_: " << pduTofBiasP5V0Cur_ << "\n";
  stream << "pduTofBiasTemp_: " << pduTofBiasTemp_ << ", pduMainDCDCTemp_: " << pduMainDCDCTemp_ << "\n";

  {
    const size_t n = sizeof(rtdGondolaFrame_) / sizeof(rtdGondolaFrame_[0]);
    stream << "rtdsInsideChamber_: ";
    for (size_t i = 0; i < n; ++i)
      stream << rtdGondolaFrame_[i] << (i + 1 < n ? ", " : "\n");
  }
  stream << "rtdDaqCrate1_: " << rtdDaqCrate1_ << ", rtdDaqCrate2_: " << rtdDaqCrate2_ << "\n";
  stream << "rtdDaqCrateBackup_: " << rtdDaqCrateBackup_ << ", rtdShaperFaradayCage1_: " << rtdShaperFaradayCage1_ << "\n";
  stream << "rtdShaperFaradayCage2_: " << rtdShaperFaradayCage2_ << "\n";

  {
    const size_t n = sizeof(rtdShaperBoard_) / sizeof(rtdShaperBoard_[0]);
    stream << "rtdShaperBoard: ";
    for (size_t i = 0; i < n; ++i) {
      stream << rtdShaperBoard_[i] << (i + 1 < n ? ", " : "\n");
    }
  }
  stream << "rtdHubComputerLocation2_: " << rtdHubComputerLocation2_ << ", rtdTofFpgas_: " << rtdTofFpgas_ << "\n";
  stream << "rtdTof2_: " << rtdTof2_ << ", rtdSealedEnclosure1WaterTank_: " << rtdSealedEnclosure1WaterTank_ << "\n";
  stream << "rtdSealedEnclosureLocation2_: " << rtdSealedEnclosureLocation2_ << ", rtdVacuumJacket1_: " << rtdVacuumJacket1_ << "\n";
  stream << "rtdVacuumJacket2_: " << rtdVacuumJacket2_ << ", rtdVacuumJacket3_: " << rtdVacuumJacket3_ << "\n";

  stream << "pressureRegulator_: " << pressureRegulator_ << ", pressureTransducer_: " << pressureTransducer_ << "\n";
  stream << "liquidLevelMeter_: " << liquidLevelMeter_ << ", inclinometer_: " << inclinometer_ << "\n";

  {
    const size_t n = sizeof(rtdsInsideChamber_) / sizeof(rtdsInsideChamber_[0]);
    stream << "rtdsInsideChamber_: ";
    for (size_t i = 0; i < n; ++i) {
      stream << rtdsInsideChamber_[i] << (i + 1 < n ? ", " : "\n");
    }
  }

  {
    const size_t n = sizeof(spare_) / sizeof(spare_[0]);
    stream << "spare_: ";
    for (size_t i = 0; i < n; ++i) {
      stream << spare_[i] << (i + 1 < n ? ", " : "\n");
    }
  }

  stream << "tofBiasVoltage_: ";
  for (size_t i = 0; i < NUM_TOF_BIAS; ++i) {
    stream << tofBiasVoltage_[i] << (i + 1 < NUM_TOF_BIAS ? ", " : "\n");
  }

  stream << "hubComputerErrorFlags_: ";
  for (size_t i = 0; i < NUM_ERROR_FLAGS; ++i) {
    stream << hubComputerErrorFlags_[i] << (i + 1 < NUM_ERROR_FLAGS ? ", " : "\n");
  }

  stream << "storageSize_: " << storageSize_ << ", cpuTemperature_: " << cpuTemperature_ << ", ramUsage_: " << ramUsage_ << "\n";
  return stream;
}
} // namespace gramsballoon::pgrams
