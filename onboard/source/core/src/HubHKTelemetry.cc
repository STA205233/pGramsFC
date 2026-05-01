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

namespace {
template <typename Array, typename Func, size_t... I>
void forEachArrayElementImpl(Array &&array, Func &&func, std::index_sequence<I...>) {
  (func(std::get<I>(array), I), ...);
}

template <typename Array, typename Func>
void forEachArrayElement(Array &&array, Func &&func) {
  using ArrayType = std::remove_reference_t<Array>;
  forEachArrayElementImpl(std::forward<Array>(array), std::forward<Func>(func), std::make_index_sequence<std::tuple_size_v<ArrayType>>{});
}
} // namespace

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
  forEachArrayElement(pduVolSiPM_, [&](const auto &value, size_t i) {
    sink->setFieldValue("pdu_vol_sipm_" + std::to_string(i), value);
  });
  forEachArrayElement(pduCurSiPM_, [&](const auto &value, size_t i) {
    sink->setFieldValue("pdu_cur_sipm_" + std::to_string(i), value);
  });

  sink->setFieldValue("pdu_cur_tpc_hv", pduCurTPCHV_);
  forEachArrayElement(pduHVTemp_, [&](const auto &value, size_t i) {
    sink->setFieldValue("pdu_hv_temp_" + std::to_string(i), value);
  });

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
  forEachArrayElement(pduToFTelemetry_, [&](const auto &value, size_t i) {
    sink->setFieldValue("pdu_tof_telemetry_" + std::to_string(i), value);
  });

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

  forEachArrayElement(rtdGondolaFrame_, [&](const auto &value, size_t i) {
    sink->setFieldValue("rtd_gondola_frame_" + std::to_string(i), value);
  });

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
  sink->setFieldValue("liquid_level_meter", 0);
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
  sink->addField("command_rejected_index_hub", static_cast<uint32_t>(0));
  sink->addField("last_command_code_orc", static_cast<uint32_t>(0));
  sink->addField("last_command_index_orc", static_cast<uint16_t>(0));
  sink->addField("command_rejected_index_orc", static_cast<uint32_t>(0));
  sink->addField("last_command_code_tpc", static_cast<uint32_t>(0));
  sink->addField("last_command_index_tpc", static_cast<uint16_t>(0));
  sink->addField("command_rejected_index_tpc", static_cast<uint32_t>(0));
  sink->addField("last_command_code_tof", static_cast<uint32_t>(0));
  sink->addField("last_command_index_tof", static_cast<uint16_t>(0));
  sink->addField("command_rejected_index_tof", static_cast<uint32_t>(0));
  sink->addField("last_command_code_qm", static_cast<uint32_t>(0));
  sink->addField("last_command_index_qm", static_cast<uint16_t>(0));
  sink->addField("command_rejected_index_qm", static_cast<uint32_t>(0));
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
  DivideData(static_cast<uint32_t>(contents->getArguments(21)), pduChargePreAmpTemp_, std::get<0>(pduToFTelemetry_));
  DivideData(static_cast<uint32_t>(contents->getArguments(22)), std::get<1>(pduToFTelemetry_), std::get<2>(pduToFTelemetry_));
  DivideData(static_cast<uint32_t>(contents->getArguments(23)), std::get<3>(pduToFTelemetry_), std::get<4>(pduToFTelemetry_));
  DivideData(static_cast<uint32_t>(contents->getArguments(24)), std::get<5>(pduToFTelemetry_), pduCaenNevisP12VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(25)), pduCaenNevisP12VCur_, pduCaenNevisM5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(26)), pduCaenNevisM5VCur_, pduCaenNevisP5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(27)), pduCaenNevisP5VCur_, pduCaenNevisP3V3Vol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(28)), pduCaenNevisP3V3Cur_, pduWarmTPCShaperPVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(29)), pduCaenNevisPM5VTemp_, pduWarmTPCShaperTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(30)), pduWarmTPCShaperMVol_, std::get<0>(pduWarmTPCShaperPCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(31)), std::get<1>(pduWarmTPCShaperPCur_), std::get<2>(pduWarmTPCShaperPCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(32)), std::get<3>(pduWarmTPCShaperPCur_), std::get<4>(pduWarmTPCShaperPCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(33)), std::get<5>(pduWarmTPCShaperPCur_), std::get<0>(pduWarmTPCShaperMCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(34)), std::get<1>(pduWarmTPCShaperMCur_), std::get<2>(pduWarmTPCShaperMCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(35)), std::get<3>(pduWarmTPCShaperMCur_), std::get<4>(pduWarmTPCShaperMCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(36)), std::get<5>(pduWarmTPCShaperMCur_), std::get<0>(pduCPUVol_));
  DivideData(static_cast<uint32_t>(contents->getArguments(37)), std::get<1>(pduCPUVol_), std::get<2>(pduCPUVol_));
  DivideData(static_cast<uint32_t>(contents->getArguments(38)), std::get<3>(pduCPUVol_), std::get<4>(pduCPUVol_));
  DivideData(static_cast<uint32_t>(contents->getArguments(39)), std::get<5>(pduCPUVol_), std::get<6>(pduCPUVol_));
  DivideData(static_cast<uint32_t>(contents->getArguments(40)), std::get<0>(pduCPUCur_), std::get<1>(pduCPUCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(41)), std::get<2>(pduCPUCur_), std::get<3>(pduCPUCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(42)), std::get<4>(pduCPUCur_), std::get<5>(pduCPUCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(43)), std::get<6>(pduCPUCur_), reserved1_);
  DivideData(static_cast<uint32_t>(contents->getArguments(44)), reserved2_, pduTofP5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(45)), pduTofP5VCur_, pduTofTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(46)), pduTofBiasP5V0Vol_, pduTofBiasP5V0Cur_);
  DivideData(static_cast<uint32_t>(contents->getArguments(47)), pduTofBiasTemp_, pduMainDCDCTemp_);
  // MHADC
  DivideData(static_cast<uint32_t>(contents->getArguments(48)), std::get<0>(rtdGondolaFrame_), std::get<1>(rtdGondolaFrame_));
  DivideData(static_cast<uint32_t>(contents->getArguments(49)), std::get<2>(rtdGondolaFrame_), std::get<3>(rtdGondolaFrame_));
  DivideData(static_cast<uint32_t>(contents->getArguments(50)), rtdDaqCrate1_, rtdDaqCrate2_);
  DivideData(static_cast<uint32_t>(contents->getArguments(51)), rtdDaqCrateBackup_, rtdShaperFaradayCage1_);
  DivideData(static_cast<uint32_t>(contents->getArguments(52)), rtdShaperFaradayCage2_, std::get<0>(rtdShaperBoard_));
  DivideData(static_cast<uint32_t>(contents->getArguments(53)), std::get<1>(rtdShaperBoard_), std::get<2>(rtdShaperBoard_));
  DivideData(static_cast<uint32_t>(contents->getArguments(54)), std::get<3>(rtdShaperBoard_), std::get<4>(rtdShaperBoard_));
  DivideData(static_cast<uint32_t>(contents->getArguments(55)), std::get<5>(rtdShaperBoard_), rtdHubComputerLocation1_);
  DivideData(static_cast<uint32_t>(contents->getArguments(56)), rtdHubComputerLocation2_, rtdTofFpgas_);
  DivideData(static_cast<uint32_t>(contents->getArguments(57)), rtdTof2_, rtdSealedEnclosure1WaterTank_);
  DivideData(static_cast<uint32_t>(contents->getArguments(58)), rtdSealedEnclosureLocation2_, rtdVacuumJacket1_);
  DivideData(static_cast<uint32_t>(contents->getArguments(59)), rtdVacuumJacket2_, rtdVacuumJacket3_);
  DivideData(static_cast<uint32_t>(contents->getArguments(60)), pressureRegulator_, inclinometer_);
  DivideData(static_cast<uint32_t>(contents->getArguments(61)), std::get<0>(rtdsInsideChamber_), std::get<1>(rtdsInsideChamber_));
  DivideData(static_cast<uint32_t>(contents->getArguments(62)), std::get<2>(rtdsInsideChamber_), std::get<3>(rtdsInsideChamber_));
  DivideData(static_cast<uint32_t>(contents->getArguments(63)), std::get<4>(rtdsInsideChamber_), std::get<5>(rtdsInsideChamber_));
  DivideData(static_cast<uint32_t>(contents->getArguments(64)), std::get<6>(rtdsInsideChamber_), std::get<0>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(65)), std::get<1>(spare_), std::get<2>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(66)), std::get<3>(spare_), std::get<4>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(67)), std::get<5>(spare_), std::get<6>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(68)), std::get<7>(spare_), std::get<8>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(69)), std::get<9>(spare_), std::get<10>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(70)), sealedEnclosurePressure_, sealedEnclosureTemperature_);
  DivideData(static_cast<uint32_t>(contents->getArguments(71)), sealedEnclosureHumidity_, std::get<0>(pressureSensors_));
  DivideData(static_cast<uint32_t>(contents->getArguments(72)), std::get<1>(pressureSensors_), std::get<0>(rtd4Wire_));
  DivideData(static_cast<uint32_t>(contents->getArguments(73)), std::get<1>(rtd4Wire_));

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
  setArguments(8, CompileData(lastCommandCodeQM_, std::get<0>(pduVolSiPM_)));
  setArguments(9, CompileData(std::get<1>(pduVolSiPM_), std::get<2>(pduVolSiPM_)));
  setArguments(10, CompileData(std::get<3>(pduVolSiPM_), std::get<4>(pduVolSiPM_)));
  setArguments(11, CompileData(std::get<5>(pduVolSiPM_), std::get<0>(pduCurSiPM_)));
  setArguments(12, CompileData(std::get<1>(pduCurSiPM_), std::get<2>(pduCurSiPM_)));
  setArguments(13, CompileData(std::get<3>(pduCurSiPM_), std::get<4>(pduCurSiPM_)));
  setArguments(14, CompileData(std::get<5>(pduCurSiPM_), pduCurTPCHV_));
  setArguments(15, CompileData(std::get<0>(pduHVTemp_), std::get<1>(pduHVTemp_)));
  setArguments(16, CompileData(pduComsBoardTemp_, pduSiPMPreAmpP2V5Vol_));
  setArguments(17, CompileData(pduSiPMPreAmpP2V5Cur_, pduSiPMPreAmpM5VVol_));
  setArguments(18, CompileData(pduSiPMPreAmpM5VCur_, pduSiPMPreAmpTemp_));
  setArguments(19, CompileData(pduChargePreAmpP5VVol_, pduChargePreAmpP5VCur_));
  setArguments(20, CompileData(pduChargePreAmpM5VVol_, pduChargePreAmpM5VCur_));
  setArguments(21, CompileData(pduChargePreAmpTemp_, std::get<0>(pduToFTelemetry_)));
  setArguments(22, CompileData(std::get<1>(pduToFTelemetry_), std::get<2>(pduToFTelemetry_)));
  setArguments(23, CompileData(std::get<3>(pduToFTelemetry_), std::get<4>(pduToFTelemetry_)));
  setArguments(24, CompileData(std::get<5>(pduToFTelemetry_), pduCaenNevisP12VVol_));
  setArguments(25, CompileData(pduCaenNevisP12VCur_, pduCaenNevisM5VVol_));
  setArguments(26, CompileData(pduCaenNevisM5VCur_, pduCaenNevisP5VVol_));
  setArguments(27, CompileData(pduCaenNevisP5VCur_, pduCaenNevisP3V3Vol_));
  setArguments(28, CompileData(pduCaenNevisP3V3Cur_, pduWarmTPCShaperPVol_));
  setArguments(29, CompileData(pduCaenNevisPM5VTemp_, pduWarmTPCShaperTemp_));
  setArguments(30, CompileData(pduWarmTPCShaperMVol_, std::get<0>(pduWarmTPCShaperPCur_)));
  setArguments(31, CompileData(std::get<1>(pduWarmTPCShaperPCur_), std::get<2>(pduWarmTPCShaperPCur_)));
  setArguments(32, CompileData(std::get<3>(pduWarmTPCShaperPCur_), std::get<4>(pduWarmTPCShaperPCur_)));
  setArguments(33, CompileData(std::get<5>(pduWarmTPCShaperPCur_), std::get<0>(pduWarmTPCShaperMCur_)));
  setArguments(34, CompileData(std::get<1>(pduWarmTPCShaperMCur_), std::get<2>(pduWarmTPCShaperMCur_)));
  setArguments(35, CompileData(std::get<3>(pduWarmTPCShaperMCur_), std::get<4>(pduWarmTPCShaperMCur_)));
  setArguments(36, CompileData(std::get<5>(pduWarmTPCShaperMCur_), std::get<0>(pduCPUVol_)));
  setArguments(37, CompileData(std::get<1>(pduCPUVol_), std::get<2>(pduCPUVol_)));
  setArguments(38, CompileData(std::get<3>(pduCPUVol_), std::get<4>(pduCPUVol_)));
  setArguments(39, CompileData(std::get<5>(pduCPUVol_), std::get<6>(pduCPUVol_)));
  setArguments(40, CompileData(std::get<0>(pduCPUCur_), std::get<1>(pduCPUCur_)));
  setArguments(41, CompileData(std::get<2>(pduCPUCur_), std::get<3>(pduCPUCur_)));
  setArguments(42, CompileData(std::get<4>(pduCPUCur_), std::get<5>(pduCPUCur_)));
  setArguments(43, CompileData(std::get<6>(pduCPUCur_), reserved1_));
  setArguments(44, CompileData(reserved2_, pduTofP5VVol_));
  setArguments(45, CompileData(pduTofP5VCur_, pduTofTemp_));
  setArguments(46, CompileData(pduTofBiasP5V0Vol_, pduTofBiasP5V0Cur_));
  setArguments(47, CompileData(pduTofBiasTemp_, pduMainDCDCTemp_));

  // MHADC
  setArguments(48, CompileData(std::get<0>(rtdGondolaFrame_), std::get<1>(rtdGondolaFrame_)));
  setArguments(49, CompileData(std::get<2>(rtdGondolaFrame_), std::get<3>(rtdGondolaFrame_)));
  setArguments(50, CompileData(rtdDaqCrate1_, rtdDaqCrate2_));
  setArguments(51, CompileData(rtdDaqCrateBackup_, rtdShaperFaradayCage1_));
  setArguments(52, CompileData(rtdShaperFaradayCage2_, std::get<0>(rtdShaperBoard_)));
  setArguments(53, CompileData(std::get<1>(rtdShaperBoard_), std::get<2>(rtdShaperBoard_)));
  setArguments(54, CompileData(std::get<3>(rtdShaperBoard_), std::get<4>(rtdShaperBoard_)));
  setArguments(55, CompileData(std::get<5>(rtdShaperBoard_), rtdHubComputerLocation1_));
  setArguments(56, CompileData(rtdHubComputerLocation2_, rtdTofFpgas_));
  setArguments(57, CompileData(rtdTof2_, rtdSealedEnclosure1WaterTank_));
  setArguments(58, CompileData(rtdSealedEnclosureLocation2_, rtdVacuumJacket1_));
  setArguments(59, CompileData(rtdVacuumJacket2_, rtdVacuumJacket3_));
  setArguments(60, CompileData(pressureRegulator_, pressureTransducer_));
  setArguments(62, CompileData(std::get<0>(rtdsInsideChamber_), std::get<1>(rtdsInsideChamber_)));
  setArguments(63, CompileData(std::get<2>(rtdsInsideChamber_), std::get<3>(rtdsInsideChamber_)));
  setArguments(64, CompileData(std::get<4>(rtdsInsideChamber_), std::get<5>(rtdsInsideChamber_)));
  setArguments(65, CompileData(std::get<6>(rtdsInsideChamber_), std::get<0>(spare_)));
  setArguments(66, CompileData(std::get<1>(spare_), std::get<2>(spare_)));
  setArguments(67, CompileData(std::get<3>(spare_), std::get<4>(spare_)));
  setArguments(68, CompileData(std::get<5>(spare_), std::get<6>(spare_)));
  setArguments(69, CompileData(std::get<7>(spare_), std::get<8>(spare_)));
  setArguments(70, CompileData(std::get<9>(spare_), std::get<10>(spare_)));
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
