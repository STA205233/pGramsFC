#include "HubHKTelemetry.hh"
#include "ConversionConstant.hh"
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

  // PDU telemetry
  for (size_t i = 0; i < NUM_PDU_SIPM; ++i) {
    sink->setFieldValue("pdu_vol_sipm_" + std::to_string(i), pduVolSiPM_[i]);
    sink->setFieldValue("pdu_cur_sipm_" + std::to_string(i), pduCurSiPM_[i]);
  }
  sink->setFieldValue("pdu_cur_tpchv", pduCurTPCHV_);
  sink->setFieldValue("pdu_vol_tpchv", pduVolTPCHV_);
  for (size_t i = 0; i < NUM_PDU_HV_TEMP; ++i) {
    sink->setFieldValue("pdu_hv_temp_" + std::to_string(i), pduHVTemp_[i]);
  }
  sink->setFieldValue("pdu_comms_board_temp", pduCommsBoardTemp_);
  sink->setFieldValue("pdu_sipm_preamp_p2v5_vol", pduSiPMPreAmpP2V5Vol_);
  sink->setFieldValue("pdu_sipm_preamp_p2v5_cur", pduSiPMPreAmpP2V5Cur_);
  sink->setFieldValue("pdu_sipm_preamp_m5v_vol", pduSiPMPreAmpM5VVol_);
  sink->setFieldValue("pdu_sipm_preamp_m5v_cur", pduSiPMPreAmpM5VCur_);
  sink->setFieldValue("pdu_sipm_preamp_temp", pduSiPMPreAmpTemp_);
  sink->setFieldValue("pdu_charge_preamp_p5v_vol", pduChargePreAmpP5VVol_);
  sink->setFieldValue("pdu_charge_preamp_p5v_cur", pduChargePreAmpP5VCur_);
  sink->setFieldValue("pdu_charge_preamp_m5v_vol", pduChargePreAmpM5VVol_);
  sink->setFieldValue("pdu_charge_preamp_m5v_cur", pduChargePreAmpM5VCur_);
  sink->setFieldValue("pdu_charge_preamp_temp", pduChargePreAmpTemp_);
  sink->setFieldValue("pdu_tof_bias_p5v0_vol", pduTofBiasP5V0Vol_);
  sink->setFieldValue("pdu_tof_bias_p5v0_cur", pduTofBiasP5V0Cur_);
  sink->setFieldValue("pdu_tof_bias_p5v1_vol", pduTofBiasP5V1Vol_);
  sink->setFieldValue("pdu_tof_bias_p5v1_cur", pduTofBiasP5V1Cur_);
  sink->setFieldValue("pdu_tof_bias_p5v_temp", pduTofBiasP5VTemp_);
  sink->setFieldValue("pdu_tof_p12v_vol", pduTofP12VVol_);
  sink->setFieldValue("pdu_tof_p12v_cur", pduTofP12VCur_);
  sink->setFieldValue("pdu_caen_nevis_p12v_vol", pduCaenNevisP12VVol_);
  sink->setFieldValue("pdu_caen_nevis_p12v_cur", pduCaenNevisP12VCur_);
  sink->setFieldValue("pdu_caen_nevis_m5v_vol", pduCaenNevisM5VVol_);
  sink->setFieldValue("pdu_caen_nevis_m5v_cur", pduCaenNevisM5VCur_);
  sink->setFieldValue("pdu_caen_nevis_p5v_vol", pduCaenNevisP5VVol_);
  sink->setFieldValue("pdu_caen_nevis_p5v_cur", pduCaenNevisP5VCur_);
  sink->setFieldValue("pdu_caen_nevis_p3v3_vol", pduCaenNevisP3V3Vol_);
  sink->setFieldValue("pdu_caen_nevis_p3v3_cur", pduCaenNevisP3V3Cur_);
  sink->setFieldValue("pdu_shaper_p3v3_vol", pduShaperP3V3Vol_);
  sink->setFieldValue("pdu_caen_nevis_pm5v_temp", pduCaenNevisPM5VTemp_);
  sink->setFieldValue("pdu_caen_nevis_p12v_temp", pduCaenNevisP12VTemp_);
  sink->setFieldValue("pdu_caen_nevis_p3v3_temp", pduCaenNevisP3V3Temp_);
  sink->setFieldValue("pdu_shaper_temp", pduShaperTemp_);
  sink->setFieldValue("pdu_shaper_m3v3_vol", pduShaperM3V3Vol_);

  for (size_t i = 0; i < NUM_PDU_WARM_TPC_SHAPER; ++i) {
    sink->setFieldValue("pdu_shaper_p_cur_" + std::to_string(i), pduShaperPCur_[i]);
    sink->setFieldValue("pdu_shaper_m_cur_" + std::to_string(i), pduShaperMCur_[i]);
  }

  for (size_t i = 0; i < NUM_PDU_CPU; ++i) {
    sink->setFieldValue("pdu_cpu_cur_" + std::to_string(i), pduCPUCur_[i]);
    sink->setFieldValue("pdu_cpu_vol_" + std::to_string(i), pduCPUVol_[i]);
  }
  sink->setFieldValue("pdu_cpu_unused_cur", pduCpuUnusedCur_);

  sink->setFieldValue("pressure_regulator_vol", pressureRegulatorVol_);
  sink->setFieldValue("pdu_tof_p12v_temp", pduTofP12VTemp_);

  sink->setFieldValue("pdu_main_bat_temp", pduMainBatTemp_);
  sink->setFieldValue("pdu_main_bat_cur", pduCurMainBat_);
  sink->setFieldValue("pdu_main_bat_vol", pduVolMainBat_);

  // MHADC
  for (size_t i = 0; i < NUM_RTD_GONDOLA; ++i) {
    sink->setFieldValue("rtd_gondola_frame_" + std::to_string(i), rtdGondolaFrame_[i]);
  }
  for (size_t i = 0; i < NUM_RTD_DAQ_CRATE; ++i) {
    sink->setFieldValue("rtd_daq_crate_" + std::to_string(i), rtdDaqCrate_[i]);
  }
  for (size_t i = 0; i < NUM_RTD_SHAPER_FARADAY_CAGE; ++i) {
    sink->setFieldValue("rtd_shaper_faraday_cage_" + std::to_string(i), rtdShaperFaradayCage_[i]);
  }
  for (size_t i = 0; i < NUM_RTD_SHAPER_BOARD; ++i) {
    sink->setFieldValue("rtd_shaper_board_" + std::to_string(i), rtdShaperBoard_[i]);
  }

  for (size_t i = 0; i < NUM_RTD_HUB_COMPUTER_LOCATION; ++i) {
    sink->setFieldValue("rtd_hub_computer_location_" + std::to_string(i), rtdHubComputerLocation_[i]);
  }

  sink->setFieldValue("rtd_tof_fpgas", rtdTofFpga_);
  sink->setFieldValue("rtd_tof", rtdTof_);
  for (size_t i = 0; i < NUM_RTD_OUTSIDE_SEALED_ENCLOSURE; ++i) {
    sink->setFieldValue("rtd_outside_sealed_enclosure_" + std::to_string(i), rtdOutsideSealedEnclosure_[i]);
  }
  for (size_t i = 0; i < NUM_RTD_VACUUM_JACKET; ++i) {
    sink->setFieldValue("rtd_vacuum_jacket_" + std::to_string(i), rtdVacuumJacket_[i]);
  }
  for (size_t i = 0; i < NUM_INCLINOMETERS; ++i) {
    sink->setFieldValue("inclinometer_" + std::to_string(i), inclinometers_[i]);
  }

  for (size_t i = 0; i < NUM_RTD_INSIDE_CHAMBER; ++i) {
    sink->setFieldValue("rtd_inside_chamber_" + std::to_string(i), rtdsInsideChamber_[i]);
  }

  for (size_t i = 0; i < NUM_ADC_SPARE; ++i) {
    sink->setFieldValue("spare_" + std::to_string(i), spare_[i]);
  }

  sink->setFieldValue("sealed_enclosure_pressure", sealedEnclosurePressure_);
  sink->setFieldValue("sealed_enclosure_temperature", sealedEnclosureTemperature_);
  sink->setFieldValue("sealed_enclosure_humidity", sealedEnclosureHumidity_);
  for (size_t i = 0; i < NUM_PRESSURE_SENSORS; ++i) {
    sink->setFieldValue("pressure_sensor_" + std::to_string(i), pressureSensors_[i]);
  }
  sink->setFieldValue("lab_jack_temperature", labJackTemperature_);
  for (size_t i = 0; i < NUM_4_WIRE_RTD; ++i) {
    sink->setFieldValue("rtd_4_wire_" + std::to_string(i), rtd4Wire_[i]);
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
  sink->addField("last_command_code_hub", static_cast<uint16_t>(0));
  sink->addField("last_command_index_hub", static_cast<uint32_t>(0));
  sink->addField("command_rejected_index_hub", static_cast<uint32_t>(0));
  sink->addField("last_command_code_orc", static_cast<uint16_t>(0));
  sink->addField("last_command_index_orc", static_cast<uint32_t>(0));
  sink->addField("command_rejected_index_orc", static_cast<uint32_t>(0));
  sink->addField("last_command_code_tpc", static_cast<uint16_t>(0));
  sink->addField("last_command_index_tpc", static_cast<uint32_t>(0));
  sink->addField("command_rejected_index_tpc", static_cast<uint32_t>(0));
  sink->addField("last_command_code_tof", static_cast<uint16_t>(0));
  sink->addField("last_command_index_tof", static_cast<uint32_t>(0));
  sink->addField("command_rejected_index_tof", static_cast<uint32_t>(0));
  sink->addField("last_command_code_qm", static_cast<uint16_t>(0));
  sink->addField("last_command_index_qm", static_cast<uint32_t>(0));
  sink->addField("command_rejected_index_qm", static_cast<uint32_t>(0));

  // PDU telemetry
  for (size_t i = 0; i < NUM_PDU_SIPM; ++i) {
    sink->addField("pdu_vol_sipm_" + std::to_string(i), static_cast<uint16_t>(0));
    sink->addField("pdu_cur_sipm_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("pdu_cur_tpchv", static_cast<uint16_t>(0));
  sink->addField("pdu_vol_tpchv", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_PDU_HV_TEMP; ++i) {
    sink->addField("pdu_hv_temp_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("pdu_comms_board_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_preamp_p2v5_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_preamp_p2v5_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_preamp_m5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_preamp_m5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_sipm_preamp_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_preamp_p5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_preamp_p5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_preamp_m5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_preamp_m5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_charge_preamp_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_bias_p5v0_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_bias_p5v0_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_bias_p5v1_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_bias_p5v1_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_bias_p5v_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_p12v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_p12v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p12v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p12v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_m5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_m5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p5v_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p5v_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p3v3_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p3v3_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_shaper_p3v3_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_pm5v_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p12v_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_caen_nevis_p3v3_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_shaper_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_shaper_m3v3_vol", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_PDU_WARM_TPC_SHAPER; ++i) {
    sink->addField("pdu_shaper_p_cur_" + std::to_string(i), static_cast<uint16_t>(0));
    sink->addField("pdu_shaper_m_cur_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_PDU_CPU; ++i) {
    sink->addField("pdu_cpu_cur_" + std::to_string(i), static_cast<uint16_t>(0));
    sink->addField("pdu_cpu_vol_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("pdu_cpu_unused_cur", static_cast<uint16_t>(0));
  sink->addField("pressure_regulator_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_tof_p12v_temp", static_cast<uint16_t>(0));
  sink->addField("pdu_main_bat_cur", static_cast<uint16_t>(0));
  sink->addField("pdu_main_bat_vol", static_cast<uint16_t>(0));
  sink->addField("pdu_main_bat_temp", static_cast<uint16_t>(0));

  // MHADC
  for (size_t i = 0; i < NUM_RTD_GONDOLA; ++i) {
    sink->addField("rtd_gondola_frame_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_DAQ_CRATE; ++i) {
    sink->addField("rtd_daq_crate_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_SHAPER_FARADAY_CAGE; ++i) {
    sink->addField("rtd_shaper_faraday_cage_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_SHAPER_BOARD; ++i) {
    sink->addField("rtd_shaper_board_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_HUB_COMPUTER_LOCATION; ++i) {
    sink->addField("rtd_hub_computer_location_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("rtd_tof_fpgas", static_cast<uint16_t>(0));
  sink->addField("rtd_tof", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_RTD_OUTSIDE_SEALED_ENCLOSURE; ++i) {
    sink->addField("rtd_outside_sealed_enclosure_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_VACUUM_JACKET; ++i) {
    sink->addField("rtd_vacuum_jacket_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  //sink->addField("pressure_transducer", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_INCLINOMETERS; ++i) {
    sink->addField("inclinometer_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_INSIDE_CHAMBER; ++i) {
    sink->addField("rtd_inside_chamber_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_ADC_SPARE; ++i) {
    sink->addField("spare_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("sealed_enclosure_pressure", static_cast<uint16_t>(0));
  sink->addField("sealed_enclosure_temperature", static_cast<uint16_t>(0));
  sink->addField("sealed_enclosure_humidity", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_PRESSURE_SENSORS; ++i) {
    sink->addField("pressure_sensor_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  sink->addField("lab_jack_temperature", static_cast<uint16_t>(0));
  for (size_t i = 0; i < NUM_4_WIRE_RTD; ++i) {
    sink->addField("rtd_4_wire_" + std::to_string(i), static_cast<uint16_t>(0));
  }
  for (size_t i = 0; i < NUM_ERROR_FLAGS; ++i) {
    sink->addField("error_flag_" + std::to_string(i), static_cast<uint32_t>(0));
  }
  sink->addField("storage_size", static_cast<uint32_t>(0));
  sink->addField("cpu_temperature", static_cast<uint16_t>(0));
  sink->addField("ram_usage", static_cast<uint16_t>(0));
}

template <typename Contents, size_t... Is>
void HubHKTelemetry::interpretErrorFlags(const Contents *contents, std::index_sequence<Is...>) {
  ((std::get<Is>(hubComputerErrorFlags_) = static_cast<uint32_t>(contents->getArguments(ARG_INDEX_TOF_BIAS + Is))), ...);
}
template <size_t... Is>
void HubHKTelemetry::updateErrorFlags(std::index_sequence<Is...>) {
  (setArguments(ARG_INDEX_TOF_BIAS + Is, std::get<Is>(hubComputerErrorFlags_)), ...);
}

template <typename Stream, typename Contents, size_t... Is>
Stream &printIterativeImpl(Stream &stream, const Contents &contents, std::index_sequence<Is...>) {
  ([](Stream &s, const Contents &c) -> void { s << std::get<Is>(c) << " "; }(stream, contents), ...);
  stream << std::endl;
  return stream;
}

template <size_t N, typename Stream, typename Contents>
Stream &printIterative(Stream &stream, const Contents &contents) {
  return printIterativeImpl(stream, contents, std::make_index_sequence<N>{});
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
  DivideData(static_cast<uint32_t>(contents->getArguments(8)), lastCommandCodeQM_, std::get<0>(pduVolSiPM_));
  DivideData(static_cast<uint32_t>(contents->getArguments(9)), std::get<1>(pduVolSiPM_), std::get<2>(pduVolSiPM_));
  DivideData(static_cast<uint32_t>(contents->getArguments(10)), std::get<3>(pduVolSiPM_), std::get<4>(pduVolSiPM_));
  DivideData(static_cast<uint32_t>(contents->getArguments(11)), std::get<5>(pduVolSiPM_), std::get<0>(pduCurSiPM_));
  DivideData(static_cast<uint32_t>(contents->getArguments(12)), std::get<1>(pduCurSiPM_), std::get<2>(pduCurSiPM_));
  DivideData(static_cast<uint32_t>(contents->getArguments(13)), std::get<3>(pduCurSiPM_), std::get<4>(pduCurSiPM_));
  DivideData(static_cast<uint32_t>(contents->getArguments(14)), std::get<5>(pduCurSiPM_), pduCurTPCHV_);
  DivideData(static_cast<uint32_t>(contents->getArguments(15)), pduVolTPCHV_, std::get<0>(pduHVTemp_));
  DivideData(static_cast<uint32_t>(contents->getArguments(16)), std::get<1>(pduHVTemp_), pduCommsBoardTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(17)), pduSiPMPreAmpP2V5Vol_, pduSiPMPreAmpP2V5Cur_);
  DivideData(static_cast<uint32_t>(contents->getArguments(18)), pduSiPMPreAmpM5VVol_, pduSiPMPreAmpM5VCur_);
  DivideData(static_cast<uint32_t>(contents->getArguments(19)), pduSiPMPreAmpTemp_, pduChargePreAmpP5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(20)), pduChargePreAmpP5VCur_, pduChargePreAmpM5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(21)), pduChargePreAmpM5VCur_, pduChargePreAmpTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(22)), pduTofBiasP5V0Cur_, pduTofBiasP5V0Vol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(23)), pduTofBiasP5V1Cur_, pduTofBiasP5V1Vol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(24)), pduTofBiasP5VTemp_, pduTofP12VCur_);
  DivideData(static_cast<uint32_t>(contents->getArguments(25)), pduTofP12VVol_, pduCaenNevisP12VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(26)), pduCaenNevisP12VCur_, pduCaenNevisM5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(27)), pduCaenNevisM5VCur_, pduCaenNevisP5VVol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(28)), pduCaenNevisP5VCur_, pduCaenNevisP3V3Vol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(29)), pduCaenNevisP3V3Cur_, pduShaperP3V3Vol_);
  DivideData(static_cast<uint32_t>(contents->getArguments(30)), pduCaenNevisPM5VTemp_, pduCaenNevisP12VTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(31)), pduCaenNevisP3V3Temp_, pduShaperTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(32)), pduShaperM3V3Vol_, std::get<0>(pduShaperPCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(33)), std::get<1>(pduShaperPCur_), std::get<2>(pduShaperPCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(34)), std::get<3>(pduShaperPCur_), std::get<4>(pduShaperPCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(35)), std::get<5>(pduShaperPCur_), std::get<0>(pduShaperMCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(36)), std::get<1>(pduShaperMCur_), std::get<2>(pduShaperMCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(37)), std::get<3>(pduShaperMCur_), std::get<4>(pduShaperMCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(38)), std::get<5>(pduShaperMCur_), std::get<0>(pduCPUCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(39)), std::get<1>(pduCPUCur_), std::get<2>(pduCPUCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(40)), std::get<3>(pduCPUCur_), std::get<4>(pduCPUCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(41)), std::get<5>(pduCPUCur_), std::get<6>(pduCPUCur_));
  DivideData(static_cast<uint32_t>(contents->getArguments(42)), std::get<0>(pduCPUVol_), std::get<1>(pduCPUVol_));
  DivideData(static_cast<uint32_t>(contents->getArguments(43)), std::get<2>(pduCPUVol_), std::get<3>(pduCPUVol_));
  DivideData(static_cast<uint32_t>(contents->getArguments(44)), std::get<4>(pduCPUVol_), std::get<5>(pduCPUVol_));
  DivideData(static_cast<uint32_t>(contents->getArguments(45)), std::get<6>(pduCPUVol_), pduCpuUnusedCur_);
  DivideData(static_cast<uint32_t>(contents->getArguments(46)), pressureRegulatorVol_, pduTofP12VTemp_);
  DivideData(static_cast<uint32_t>(contents->getArguments(47)), pduMainBatTemp_, pduCurMainBat_);
  DivideData(static_cast<uint32_t>(contents->getArguments(48)), pduVolMainBat_, std::get<0>(rtdGondolaFrame_));
  //// MHADC
  DivideData(static_cast<uint32_t>(contents->getArguments(49)), std::get<1>(rtdGondolaFrame_), std::get<2>(rtdGondolaFrame_));
  DivideData(static_cast<uint32_t>(contents->getArguments(50)), std::get<3>(rtdGondolaFrame_), std::get<0>(rtdDaqCrate_));
  DivideData(static_cast<uint32_t>(contents->getArguments(51)), std::get<1>(rtdDaqCrate_), std::get<2>(rtdDaqCrate_));
  DivideData(static_cast<uint32_t>(contents->getArguments(52)), std::get<0>(rtdShaperFaradayCage_), std::get<1>(rtdShaperFaradayCage_));
  DivideData(static_cast<uint32_t>(contents->getArguments(53)), std::get<0>(rtdShaperBoard_), std::get<1>(rtdShaperBoard_));
  DivideData(static_cast<uint32_t>(contents->getArguments(54)), std::get<2>(rtdShaperBoard_), std::get<3>(rtdShaperBoard_));
  DivideData(static_cast<uint32_t>(contents->getArguments(55)), std::get<4>(rtdShaperBoard_), std::get<5>(rtdShaperBoard_));
  DivideData(static_cast<uint32_t>(contents->getArguments(56)), std::get<0>(rtdHubComputerLocation_), std::get<1>(rtdHubComputerLocation_));
  DivideData(static_cast<uint32_t>(contents->getArguments(57)), rtdTofFpga_, rtdTof_);
  DivideData(static_cast<uint32_t>(contents->getArguments(58)), std::get<0>(rtdOutsideSealedEnclosure_), std::get<1>(rtdOutsideSealedEnclosure_));
  DivideData(static_cast<uint32_t>(contents->getArguments(59)), std::get<0>(rtdVacuumJacket_), std::get<1>(rtdVacuumJacket_));
  DivideData(static_cast<uint32_t>(contents->getArguments(60)), std::get<2>(rtdVacuumJacket_), std::get<0>(inclinometers_));
  DivideData(static_cast<uint32_t>(contents->getArguments(61)), std::get<1>(inclinometers_), std::get<0>(rtdsInsideChamber_));
  DivideData(static_cast<uint32_t>(contents->getArguments(62)), std::get<1>(rtdsInsideChamber_), std::get<2>(rtdsInsideChamber_));
  DivideData(static_cast<uint32_t>(contents->getArguments(63)), std::get<3>(rtdsInsideChamber_), std::get<4>(rtdsInsideChamber_));
  DivideData(static_cast<uint32_t>(contents->getArguments(64)), std::get<5>(rtdsInsideChamber_), std::get<6>(rtdsInsideChamber_));
  DivideData(static_cast<uint32_t>(contents->getArguments(65)), std::get<7>(rtdsInsideChamber_), std::get<8>(rtdsInsideChamber_));
  DivideData(static_cast<uint32_t>(contents->getArguments(66)), std::get<0>(spare_), std::get<1>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(67)), std::get<2>(spare_), std::get<3>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(68)), std::get<4>(spare_), std::get<5>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(69)), std::get<6>(spare_), std::get<7>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(70)), std::get<8>(spare_), std::get<9>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(71)), std::get<10>(spare_), std::get<11>(spare_));
  DivideData(static_cast<uint32_t>(contents->getArguments(72)), std::get<12>(spare_), sealedEnclosurePressure_);
  DivideData(static_cast<uint32_t>(contents->getArguments(73)), sealedEnclosureTemperature_, sealedEnclosureHumidity_);
  DivideData(static_cast<uint32_t>(contents->getArguments(74)), std::get<0>(pressureSensors_), std::get<1>(pressureSensors_));
  DivideData(static_cast<uint32_t>(contents->getArguments(75)), labJackTemperature_, std::get<0>(rtd4Wire_));
  uint16_t dummy = 0;
  DivideData(static_cast<uint32_t>(contents->getArguments(76)), std::get<1>(rtd4Wire_), dummy);

  interpretErrorFlags(contents, std::make_index_sequence<NUM_ERROR_FLAGS>{});

  constexpr size_t INDEX_UNTIL_HERE = ARG_INDEX_TOF_BIAS + NUM_ERROR_FLAGS;
  storageSize_ = contents->getArguments(INDEX_UNTIL_HERE);

  DivideData(static_cast<uint32_t>(contents->getArguments(INDEX_UNTIL_HERE + 1)), cpuTemperature_, ramUsage_);
  commandRejectedIndexHub_ = contents->getArguments(INDEX_UNTIL_HERE + 2);
  commandRejectedIndexOrc_ = contents->getArguments(INDEX_UNTIL_HERE + 3);
  commandRejectedIndexTPC_ = contents->getArguments(INDEX_UNTIL_HERE + 4);
  commandRejectedIndexTOF_ = contents->getArguments(INDEX_UNTIL_HERE + 5);
  commandRejectedIndexQM_ = contents->getArguments(INDEX_UNTIL_HERE + 6);
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
  setArguments(15, CompileData(pduVolTPCHV_, std::get<0>(pduHVTemp_)));
  setArguments(16, CompileData(std::get<1>(pduHVTemp_), pduCommsBoardTemp_));
  setArguments(17, CompileData(pduSiPMPreAmpP2V5Vol_, pduSiPMPreAmpP2V5Cur_));
  setArguments(18, CompileData(pduSiPMPreAmpM5VVol_, pduSiPMPreAmpM5VCur_));
  setArguments(19, CompileData(pduSiPMPreAmpTemp_, pduChargePreAmpP5VVol_));
  setArguments(20, CompileData(pduChargePreAmpP5VCur_, pduChargePreAmpM5VVol_));
  setArguments(21, CompileData(pduChargePreAmpM5VCur_, pduChargePreAmpTemp_));
  setArguments(22, CompileData(pduTofBiasP5V0Cur_, pduTofBiasP5V0Vol_));
  setArguments(23, CompileData(pduTofBiasP5V1Cur_, pduTofBiasP5V1Vol_));
  setArguments(24, CompileData(pduTofBiasP5VTemp_, pduTofP12VCur_));
  setArguments(25, CompileData(pduTofP12VVol_, pduCaenNevisP12VVol_));
  setArguments(26, CompileData(pduCaenNevisP12VCur_, pduCaenNevisM5VVol_));
  setArguments(27, CompileData(pduCaenNevisM5VCur_, pduCaenNevisP5VVol_));
  setArguments(28, CompileData(pduCaenNevisP5VCur_, pduCaenNevisP3V3Vol_));
  setArguments(29, CompileData(pduCaenNevisP3V3Cur_, pduShaperP3V3Vol_));
  setArguments(30, CompileData(pduCaenNevisPM5VTemp_, pduCaenNevisP12VTemp_));
  setArguments(31, CompileData(pduCaenNevisP3V3Temp_, pduShaperTemp_));
  setArguments(32, CompileData(pduShaperM3V3Vol_, std::get<0>(pduShaperPCur_)));
  setArguments(33, CompileData(std::get<1>(pduShaperPCur_), std::get<2>(pduShaperPCur_)));
  setArguments(34, CompileData(std::get<3>(pduShaperPCur_), std::get<4>(pduShaperPCur_)));
  setArguments(35, CompileData(std::get<5>(pduShaperPCur_), std::get<0>(pduShaperMCur_)));
  setArguments(36, CompileData(std::get<1>(pduShaperMCur_), std::get<2>(pduShaperMCur_)));
  setArguments(37, CompileData(std::get<3>(pduShaperMCur_), std::get<4>(pduShaperMCur_)));
  setArguments(38, CompileData(std::get<5>(pduShaperMCur_), std::get<0>(pduCPUCur_)));
  setArguments(39, CompileData(std::get<1>(pduCPUCur_), std::get<2>(pduCPUCur_)));
  setArguments(40, CompileData(std::get<3>(pduCPUCur_), std::get<4>(pduCPUCur_)));
  setArguments(41, CompileData(std::get<5>(pduCPUCur_), std::get<6>(pduCPUCur_)));
  setArguments(42, CompileData(std::get<0>(pduCPUVol_), std::get<1>(pduCPUVol_)));
  setArguments(43, CompileData(std::get<2>(pduCPUVol_), std::get<3>(pduCPUVol_)));
  setArguments(44, CompileData(std::get<4>(pduCPUVol_), std::get<5>(pduCPUVol_)));
  setArguments(45, CompileData(std::get<6>(pduCPUVol_), pduCpuUnusedCur_));
  setArguments(46, CompileData(pressureRegulatorVol_, pduTofP12VTemp_));
  setArguments(47, CompileData(pduMainBatTemp_, pduCurMainBat_));

  // MHADC
  setArguments(48, CompileData(pduVolMainBat_, std::get<0>(rtdGondolaFrame_)));
  setArguments(49, CompileData(std::get<1>(rtdGondolaFrame_), std::get<2>(rtdGondolaFrame_)));
  setArguments(50, CompileData(std::get<3>(rtdGondolaFrame_), std::get<0>(rtdDaqCrate_)));
  setArguments(51, CompileData(std::get<1>(rtdDaqCrate_), std::get<2>(rtdDaqCrate_)));
  setArguments(52, CompileData(std::get<0>(rtdShaperFaradayCage_), std::get<1>(rtdShaperFaradayCage_)));
  setArguments(53, CompileData(std::get<0>(rtdShaperBoard_), std::get<1>(rtdShaperBoard_)));
  setArguments(54, CompileData(std::get<2>(rtdShaperBoard_), std::get<3>(rtdShaperBoard_)));
  setArguments(55, CompileData(std::get<4>(rtdShaperBoard_), std::get<5>(rtdShaperBoard_)));
  setArguments(56, CompileData(std::get<0>(rtdHubComputerLocation_), std::get<1>(rtdHubComputerLocation_)));
  setArguments(57, CompileData(rtdTofFpga_, rtdTof_));
  setArguments(58, CompileData(std::get<0>(rtdOutsideSealedEnclosure_), std::get<1>(rtdOutsideSealedEnclosure_)));
  setArguments(59, CompileData(std::get<0>(rtdVacuumJacket_), std::get<1>(rtdVacuumJacket_)));
  setArguments(60, CompileData(std::get<2>(rtdVacuumJacket_), std::get<0>(inclinometers_)));
  setArguments(61, CompileData(std::get<1>(inclinometers_), std::get<0>(rtdsInsideChamber_)));
  setArguments(62, CompileData(std::get<1>(rtdsInsideChamber_), std::get<2>(rtdsInsideChamber_)));
  setArguments(63, CompileData(std::get<3>(rtdsInsideChamber_), std::get<4>(rtdsInsideChamber_)));
  setArguments(64, CompileData(std::get<5>(rtdsInsideChamber_), std::get<6>(rtdsInsideChamber_)));
  setArguments(65, CompileData(std::get<7>(rtdsInsideChamber_), std::get<8>(rtdsInsideChamber_)));
  setArguments(66, CompileData(std::get<0>(spare_), std::get<1>(spare_)));
  setArguments(67, CompileData(std::get<2>(spare_), std::get<3>(spare_)));
  setArguments(68, CompileData(std::get<4>(spare_), std::get<5>(spare_)));
  setArguments(69, CompileData(std::get<6>(spare_), std::get<7>(spare_)));
  setArguments(70, CompileData(std::get<8>(spare_), std::get<9>(spare_)));
  setArguments(71, CompileData(std::get<10>(spare_), std::get<11>(spare_)));
  setArguments(72, CompileData(std::get<12>(spare_), sealedEnclosurePressure_));
  setArguments(73, CompileData(sealedEnclosureTemperature_, sealedEnclosureHumidity_));
  setArguments(74, CompileData(std::get<0>(pressureSensors_), std::get<1>(pressureSensors_)));
  setArguments(75, CompileData(labJackTemperature_, std::get<0>(rtd4Wire_)));
  setArguments(76, CompileData(std::get<1>(rtd4Wire_), static_cast<uint16_t>(0)));

  updateErrorFlags(std::make_index_sequence<NUM_ERROR_FLAGS>{});

  constexpr size_t INDEX_UNTIL_HERE = ARG_INDEX_TOF_BIAS + NUM_ERROR_FLAGS;
  setArguments(INDEX_UNTIL_HERE, storageSize_);
  setArguments(INDEX_UNTIL_HERE + 1, CompileData(cpuTemperature_, ramUsage_));
  setArguments(INDEX_UNTIL_HERE + 2, commandRejectedIndexHub_);
  setArguments(INDEX_UNTIL_HERE + 3, commandRejectedIndexOrc_);
  setArguments(INDEX_UNTIL_HERE + 4, commandRejectedIndexTPC_);
  setArguments(INDEX_UNTIL_HERE + 5, commandRejectedIndexTOF_);
  setArguments(INDEX_UNTIL_HERE + 6, commandRejectedIndexQM_);
  BaseTelemetryDefinition::update();
}
std::ostream &HubHKTelemetry::print(std::ostream &stream) {
  stream << "HubHKTelemetry" << std::endl;
  stream << "Code: " << getContents()->Code() << ", Argc(): " << getContents()->Argc() << std::endl;
  stream << "Data: " << std::endl;
  BaseTelemetryDefinition::print(stream);

  stream << "lastCommandIndexHub_: " << lastCommandIndexHub_
         << ", lastCommandIndexOrc_: " << lastCommandIndexOrc_
         << ", lastCommandIndexTPC_: " << lastCommandIndexTPC_
         << ", lastCommandIndexTOF_: " << lastCommandIndexTOF_
         << ", lastCommandIndexQM_: " << lastCommandIndexQM_ << std::endl;
  stream << "lastCommandCodeHub_: " << lastCommandCodeHub_
         << ", lastCommandCodeOrc_: " << lastCommandCodeOrc_
         << ", lastCommandCodeTPC_: " << lastCommandCodeTPC_
         << ", lastCommandCodeTOF_: " << lastCommandCodeTOF_
         << ", lastCommandCodeQM_: " << lastCommandCodeQM_ << std::endl;
  stream << "commandRejectedIndexHub_: " << commandRejectedIndexHub_
         << ", commandRejectedIndexOrc_: " << commandRejectedIndexOrc_
         << ", commandRejectedIndexTPC_: " << commandRejectedIndexTPC_
         << ", commandRejectedIndexTOF_: " << commandRejectedIndexTOF_
         << ", commandRejectedIndexQM_: " << commandRejectedIndexQM_ << std::endl;

  stream << "pduVolSiPM_: ";
  printIterative<NUM_PDU_SIPM>(stream, pduVolSiPM_);

  stream << "pduCurSiPM_: ";
  printIterative<NUM_PDU_SIPM>(stream, pduCurSiPM_);

  stream << "pduCurTPCHV_: " << pduCurTPCHV_ << ", pduVolTPCHV_: " << pduVolTPCHV_ << std::endl;

  stream << "pduHVTemp_: ";
  printIterative<NUM_PDU_HV_TEMP>(stream, pduHVTemp_);

  stream << "pduCommsBoardTemp_: " << pduCommsBoardTemp_ << std::endl;
  stream << "pduSiPMPreAmpP2V5Vol_: " << pduSiPMPreAmpP2V5Vol_
         << ", pduSiPMPreAmpP2V5Cur_: " << pduSiPMPreAmpP2V5Cur_
         << ", pduSiPMPreAmpM5VVol_: " << pduSiPMPreAmpM5VVol_
         << ", pduSiPMPreAmpM5VCur_: " << pduSiPMPreAmpM5VCur_
         << ", pduSiPMPreAmpTemp_: " << pduSiPMPreAmpTemp_ << std::endl;
  stream << "pduChargePreAmpP5VVol_: " << pduChargePreAmpP5VVol_
         << ", pduChargePreAmpP5VCur_: " << pduChargePreAmpP5VCur_
         << ", pduChargePreAmpM5VVol_: " << pduChargePreAmpM5VVol_
         << ", pduChargePreAmpM5VCur_: " << pduChargePreAmpM5VCur_
         << ", pduChargePreAmpTemp_: " << pduChargePreAmpTemp_ << std::endl;
  stream << "pduTofBiasP5V0Vol_: " << pduTofBiasP5V0Vol_
         << ", pduTofBiasP5V0Cur_: " << pduTofBiasP5V0Cur_
         << ", pduTofBiasP5V1Vol_: " << pduTofBiasP5V1Vol_
         << ", pduTofBiasP5V1Cur_: " << pduTofBiasP5V1Cur_
         << ", pduTofBiasP5VTemp_: " << pduTofBiasP5VTemp_ << std::endl;
  stream << "pduTofP12VVol_: " << pduTofP12VVol_
         << ", pduTofP12VCur_: " << pduTofP12VCur_ << std::endl;
  stream << "pduCaenNevisP12VVol_: " << pduCaenNevisP12VVol_
         << ", pduCaenNevisP12VCur_: " << pduCaenNevisP12VCur_
         << ", pduCaenNevisM5VVol_: " << pduCaenNevisM5VVol_
         << ", pduCaenNevisM5VCur_: " << pduCaenNevisM5VCur_
         << ", pduCaenNevisP5VVol_: " << pduCaenNevisP5VVol_
         << ", pduCaenNevisP5VCur_: " << pduCaenNevisP5VCur_
         << ", pduCaenNevisP3V3Vol_: " << pduCaenNevisP3V3Vol_
         << ", pduCaenNevisP3V3Cur_: " << pduCaenNevisP3V3Cur_ << std::endl;
  stream << "pduShaperP3V3Vol_: " << pduShaperP3V3Vol_
         << ", pduCaenNevisPM5VTemp_: " << pduCaenNevisPM5VTemp_
         << ", pduCaenNevisP12VTemp_: " << pduCaenNevisP12VTemp_
         << ", pduCaenNevisP3V3Temp_: " << pduCaenNevisP3V3Temp_
         << ", pduShaperTemp_: " << pduShaperTemp_
         << ", pduShaperM3V3Vol_: " << pduShaperM3V3Vol_ << std::endl;

  stream << "pduShaperPCur_: ";
  printIterative<NUM_PDU_WARM_TPC_SHAPER>(stream, pduShaperPCur_);

  stream << "pduShaperMCur_: ";
  printIterative<NUM_PDU_WARM_TPC_SHAPER>(stream, pduShaperMCur_);

  stream << "pduCPUCur_: ";
  printIterative<NUM_PDU_CPU>(stream, pduCPUCur_);

  stream << "pduCpuUnusedCur_: " << pduCpuUnusedCur_ << std::endl;

  stream << "pduCPUVol_: ";
  printIterative<NUM_PDU_CPU>(stream, pduCPUVol_);

  stream << "pressureRegulatorVol_: " << pressureRegulatorVol_
         << ", pduTofP12VTemp_: " << pduTofP12VTemp_
         << ", pduCurMainBat_: " << pduCurMainBat_
         << ", pduVolMainBat_: " << pduVolMainBat_
         << ", pduMainBatTemp_: " << pduMainBatTemp_ << std::endl;

  stream << "rtdGondolaFrame_: ";
  printIterative<NUM_RTD_GONDOLA>(stream, rtdGondolaFrame_);

  stream << "rtdDaqCrate_: ";
  printIterative<NUM_RTD_DAQ_CRATE>(stream, rtdDaqCrate_);

  stream << "rtdShaperFaradayCage_: ";
  printIterative<NUM_RTD_SHAPER_FARADAY_CAGE>(stream, rtdShaperFaradayCage_);

  stream << "rtdShaperBoard_: ";
  printIterative<NUM_RTD_SHAPER_BOARD>(stream, rtdShaperBoard_);

  stream << "rtdHubComputerLocation_: ";
  printIterative<NUM_RTD_HUB_COMPUTER_LOCATION>(stream, rtdHubComputerLocation_);

  stream << "rtdTofFpga_: " << rtdTofFpga_ << ", rtdTof_: " << rtdTof_ << std::endl;

  stream << "rtdOutsideSealedEnclosure_: ";
  printIterative<NUM_RTD_OUTSIDE_SEALED_ENCLOSURE>(stream, rtdOutsideSealedEnclosure_);

  stream << "rtdVacuumJacket_: ";
  printIterative<NUM_RTD_VACUUM_JACKET>(stream, rtdVacuumJacket_);

  stream << "inclinometers_: ";
  printIterative<NUM_INCLINOMETERS>(stream, inclinometers_);

  stream << "rtdsInsideChamber_: ";
  printIterative<NUM_RTD_INSIDE_CHAMBER>(stream, rtdsInsideChamber_);

  stream << "spare_: ";
  printIterative<NUM_ADC_SPARE>(stream, spare_);

  stream << "sealedEnclosurePressure_: " << sealedEnclosurePressure_
         << ", sealedEnclosureTemperature_: " << sealedEnclosureTemperature_
         << ", sealedEnclosureHumidity_: " << sealedEnclosureHumidity_ << std::endl;

  stream << "pressureSensors_: ";
  printIterative<NUM_PRESSURE_SENSORS>(stream, pressureSensors_);

  stream << "rtd4Wire_: ";
  printIterative<NUM_4_WIRE_RTD>(stream, rtd4Wire_);


  stream << "hubComputerErrorFlags_: ";
  printIterative<NUM_ERROR_FLAGS>(stream, hubComputerErrorFlags_);

  stream << "storageSize_: " << storageSize_
         << ", cpuTemperature_: " << cpuTemperature_
         << ", ramUsage_: " << ramUsage_ << std::endl;

  return stream;
}

void HubHKTelemetry::setSealedEnclosurePressure(float v) {
  setSealedEnclosurePressure(static_cast<uint16_t>(v / conversion::bme680::COEFF_BME680_PRESS));
}

void HubHKTelemetry::setSealedEnclosureTemperature(float v) {
  setSealedEnclosureTemperature(static_cast<uint16_t>(v / conversion::bme680::COEFF_BME680_TEMP));
}

void HubHKTelemetry::setSealedEnclosureHumidity(float v) {
  setSealedEnclosureHumidity(static_cast<uint16_t>(v / conversion::bme680::COEFF_BME680_HUMID));
}

void HubHKTelemetry::setLabJackTemperature(float v) {
  setLabJackTemperature(static_cast<uint16_t>(v / conversion::labjack::COEFF_TEMP));
}
} // namespace gramsballoon::pgrams
