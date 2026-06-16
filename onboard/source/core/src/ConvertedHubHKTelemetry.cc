#include "ConvertedHubHKTelemetry.hh"
#include "SystemOfUnits.hh"
using namespace gramsballoon::pgrams::units;
using floating_t = gramsballoon::pgrams::ConvertedHubHKTelemetry::floating_t;

namespace gramsballoon::pgrams {
template <typename T>
inline bool set_value(const HubHKTelemetry *raw_telemetry, T (HubHKTelemetry::*getter)() const, floating_t &value, bool (*converter)(T, floating_t &)) {
  floating_t temp_value;
  const bool ok = converter((raw_telemetry->*getter)(), temp_value);
  if (ok) { value = temp_value; }
  return ok;
}
template <size_t N, typename T>
inline bool set_all_values(const HubHKTelemetry *raw_telemetry, T (HubHKTelemetry::*getter)(size_t) const, std::array<floating_t, N> &values, bool (*converter)(T, floating_t &)) {
  bool ok = true;
  floating_t temp_value;
  for (size_t i = 0; i < N; ++i) {
    try {
      ok &= converter((raw_telemetry->*getter)(i), temp_value);
      if (ok) { values.at(i) = temp_value; }
    }
    catch (std::out_of_range &e) {
      std::cerr << "OUT OF RANGE in converting HubHKTelemetry" << std::endl;
      return false;
    }
  }
  return ok;
}
ConvertedHubHKTelemetry::ConvertedHubHKTelemetry() = default;
ConvertedHubHKTelemetry::~ConvertedHubHKTelemetry() = default;

template <typename T>
bool ConvertedHubHKTelemetry::convertVoltageMHADC(T adc_value, floating_t &src) {
  auto adc_float = static_cast<floating_t>(adc_value);
  if (adc_value >= RTD_ADC_MAX) {
    std::cerr << "Conversion failed: Open Circuit" << std::endl;
    return false;
  }
  src = adc_float * (VREF_RTD / RTD_ADC_MAX_FLOAT);
  return true;
}

template <typename T>
bool ConvertedHubHKTelemetry::convertInclinometer(T adc_value, floating_t &src) {
  floating_t voltage;
  if (!convertVoltageMHADC(adc_value, voltage)) {
    return false;
  }
  src = voltage * COEFF_INCLINOMETER;
  return true;
}

template <typename T>
bool ConvertedHubHKTelemetry::convertPDUSiPMVoltage(T adc_value, floating_t &src) {
  floating_t voltage;
  if (!convertVoltageMHADC(adc_value, voltage)) {
    return false;
  }
  src = voltage * COEFF_SIPM_VOL;
  return true;
}

template <typename T>
bool ConvertedHubHKTelemetry::convertPDUSiPMCurrent(T adc_value, floating_t &src) {
  floating_t voltage;
  if (!convertVoltagePDU(adc_value, voltage)) {
    return false;
  }
  src = voltage * COEFF_SIPM_CUR;
  return true;
}

template <typename T>
bool ConvertedHubHKTelemetry::convertPDUTPCHVCurrent(T adc_value, floating_t &src) {
  floating_t voltage;
  if (!convertVoltagePDU(adc_value, voltage)) {
    return false;
  }
  src = voltage * COEFF_TPC_HV_CUR;
  return true;
}

template <typename T>
bool ConvertedHubHKTelemetry::convertVoltagePDU(T adc_value, floating_t &src) {
  // TODO: this is preliminary
  constexpr double V_REF = 3.3 * units::volt;
  src = adc_value / 4096 * V_REF;
  return true;
}

template <typename T>
bool ConvertedHubHKTelemetry::convertRTD(T adc_value, floating_t offset, floating_t &src) {
  floating_t voltage;
  if (!convertVoltageMHADC(adc_value, voltage)) {
    return false;
  }
  floating_t L_tmp = -(std::sqrt(17.59246 - 0.00232 * voltage) - 3.908) / 0.00116 * kelvin;
  L_tmp += 273.15;
  const floating_t L_correction = L_tmp + offset;
  if (L_correction > 73) {
    src = L_correction;
    std::cerr << "Conversion failed: Short!" << std::endl;
    return true;
  }
  return false;
}

bool ConvertedHubHKTelemetry::convert(const HubHKTelemetry *raw_telemetry) {
  if (!raw_telemetry) return false;
  code_ = raw_telemetry->getContents()->Code();
  timeStamp_ = raw_telemetry->TimeStamp();
  runid_ = raw_telemetry->RunID();
  subsystem_ = raw_telemetry->Subsystem();

  lastCommandCodeHub_ = raw_telemetry->LastCommandCodeHub();
  lastCommandIndexHub_ = raw_telemetry->LastCommandIndexHub();
  commandRejectedIndexHub_ = raw_telemetry->CommandRejectedIndexHub();
  lastCommandCodeOrc_ = raw_telemetry->LastCommandCodeOrc();
  lastCommandIndexOrc_ = raw_telemetry->LastCommandIndexOrc();
  commandRejectedIndexOrc_ = raw_telemetry->CommandRejectedIndexOrc();
  lastCommandCodeTPC_ = raw_telemetry->LastCommandCodeTPC();
  lastCommandIndexTPC_ = raw_telemetry->LastCommandIndexTPC();
  commandRejectedIndexTPC_ = raw_telemetry->CommandRejectedIndexTPC();
  lastCommandCodeTOF_ = raw_telemetry->LastCommandCodeTOF();
  lastCommandIndexTOF_ = raw_telemetry->LastCommandIndexTOF();
  commandRejectedIndexTOF_ = raw_telemetry->CommandRejectedIndexTOF();
  lastCommandCodeQM_ = raw_telemetry->LastCommandCodeQM();
  lastCommandIndexQM_ = raw_telemetry->LastCommandIndexQM();
  commandRejectedIndexQM_ = raw_telemetry->CommandRejectedIndexQM();

  bool ok = true;
  // PDU
  ok &= set_all_values<NUM_PDU_SIPM, uint16_t>(raw_telemetry, &HubHKTelemetry::PduCurSiPM, pduCurSiPM_, &ConvertedHubHKTelemetry::convertPDUSiPMCurrent);
  ok &= set_all_values<NUM_PDU_SIPM, uint16_t>(raw_telemetry, &HubHKTelemetry::PduVolSiPM, pduVolSiPM_, &ConvertedHubHKTelemetry::convertPDUSiPMVoltage);
  ok &= set_value(raw_telemetry, &HubHKTelemetry::PduCurTPCHV, pduCurTPCHV_, &ConvertedHubHKTelemetry::convertPDUTPCHVCurrent);
  // MHADC
  ok &= set_all_values<NUM_INCLINOMETERS, uint16_t>(raw_telemetry, &HubHKTelemetry::Inclinometers, inclinometers_, &ConvertedHubHKTelemetry::convertInclinometer);

  // Hub computer
  hubComputerErrorFlags_ = raw_telemetry->HubComputerErrorFlags();

  return ok;
}

template <typename Stream, typename Contents, size_t... Is>
Stream &printIterativeImpl(Stream &stream, const Contents &contents, const std::string &unit_name, floating_t unit, std::index_sequence<Is...>) {
  ([&]() -> void { stream << std::get<Is>(contents) / unit << " " << unit_name << " "; }(), ...);
  stream << std::endl;
  return stream;
}

template <size_t N, typename Stream, typename Contents>
Stream &printIterative(Stream &stream, const Contents &contents, const std::string &unit_name = "", floating_t unit = 1.0) {
  return printIterativeImpl(stream, contents, unit_name, unit, std::make_index_sequence<N>{});
}

std::ostream &ConvertedHubHKTelemetry::print(std::ostream &stream) {
  stream << "ConvertedHubHKTelemetry" << std::endl;
  stream << "Time: " << timeStamp_ << std::endl;
  stream << "Index: " << index_ << std::endl;
  stream << "Subsystem: " << static_cast<int>(subsystem_) << std::endl;
  stream << "Code: " << code_ << std::endl;
  stream << "RunID: " << runid_ << std::endl;

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
  printIterative<NUM_PDU_SIPM>(stream, pduVolSiPM_, "V", units::V);

  stream << "pduCurSiPM_: ";
  printIterative<NUM_PDU_SIPM>(stream, pduCurSiPM_, "A", units::A);

  stream << "pduCurTPCHV_: " << pduCurTPCHV_ / units::mA << " mA, pduVolTPCHV_: " << pduVolTPCHV_ / V << " V" << std::endl;

  stream << "pduHVTemp_: ";
  printIterative<NUM_PDU_HV_TEMP>(stream, pduHVTemp_, "");

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

  stream << "pressureTransducer_: " << pressureTransducer_ << std::endl;

  stream << "inclinometers_: ";
  printIterative<NUM_INCLINOMETERS>(stream, inclinometers_, "degree", units::degree);

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

  stream << "tofBiasVoltage_: ";
  printIterative<NUM_TOF_BIAS>(stream, tofBiasVoltage_);

  stream << "tofBiasTemperature_: ";
  printIterative<NUM_TOF_BIAS>(stream, tofBiasTemperature_);

  stream << "hubComputerErrorFlags_: ";
  printIterative<NUM_ERROR_FLAGS>(stream, hubComputerErrorFlags_);

  stream << "storageSize_: " << storageSize_
         << ", cpuTemperature_: " << cpuTemperature_
         << ", ramUsage_: " << ramUsage_ << std::endl;

  return stream;
}
void ConvertedHubHKTelemetry::serialize(DBFieldSink *sink) const {
  sink->setFieldValue("send_time", static_cast<uint64_t>(timeStamp_));
  sink->setFieldValue("subsystem", static_cast<uint16_t>(subsystem_));
  sink->setFieldValue("telemetry_index", index_);
  sink->setFieldValue("run_id", static_cast<uint32_t>(runid_));
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

  // PDU
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
  sink->setFieldValue("pdu_main_bat_cur", pduCurMainBat_);
  sink->setFieldValue("pdu_main_bat_vol", pduVolMainBat_);
  sink->setFieldValue("pdu_main_bat_temp", pduMainBatTemp_);

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
  sink->setFieldValue("pressure_transducer", pressureTransducer_);
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
  for (size_t i = 0; i < NUM_TOF_BIAS; ++i) {
    sink->setFieldValue("tof_bias_voltage_" + std::to_string(i), tofBiasVoltage_[i]);
    sink->setFieldValue("tof_bias_temperature_" + std::to_string(i), tofBiasTemperature_[i]);
  }
  for (size_t i = 0; i < NUM_ERROR_FLAGS; ++i) {
    sink->setFieldValue("error_flag_" + std::to_string(i), hubComputerErrorFlags_[i]);
  }
  sink->setFieldValue("storage_size", storageSize_);
  sink->setFieldValue("cpu_temperature", cpuTemperature_);
  sink->setFieldValue("ram_usage", ramUsage_);
}
void ConvertedHubHKTelemetry::initializeDBTable(DBFieldSink *sink, const std::string &table_name) const {
  sink->initializeTable(table_name);
  sink->addField("send_time", static_cast<uint64_t>(timeStamp_));
  sink->addField("subsystem", static_cast<uint16_t>(subsystem_));
  sink->addField("telemetry_index", index_);
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
    sink->addField("pdu_vol_sipm_" + std::to_string(i), static_cast<floating_t>(0));
    sink->addField("pdu_cur_sipm_" + std::to_string(i), static_cast<floating_t>(0));
  }
  sink->addField("pdu_cur_tpchv", static_cast<floating_t>(0));
  sink->addField("pdu_vol_tpchv", static_cast<floating_t>(0));
  for (size_t i = 0; i < NUM_PDU_HV_TEMP; ++i) {
    sink->addField("pdu_hv_temp_" + std::to_string(i), static_cast<floating_t>(0));
  }
  sink->addField("pdu_comms_board_temp", static_cast<floating_t>(0));
  sink->addField("pdu_sipm_preamp_p2v5_vol", static_cast<floating_t>(0));
  sink->addField("pdu_sipm_preamp_p2v5_cur", static_cast<floating_t>(0));
  sink->addField("pdu_sipm_preamp_m5v_vol", static_cast<floating_t>(0));
  sink->addField("pdu_sipm_preamp_m5v_cur", static_cast<floating_t>(0));
  sink->addField("pdu_sipm_preamp_temp", static_cast<floating_t>(0));
  sink->addField("pdu_charge_preamp_p5v_vol", static_cast<floating_t>(0));
  sink->addField("pdu_charge_preamp_p5v_cur", static_cast<floating_t>(0));
  sink->addField("pdu_charge_preamp_m5v_vol", static_cast<floating_t>(0));
  sink->addField("pdu_charge_preamp_m5v_cur", static_cast<floating_t>(0));
  sink->addField("pdu_charge_preamp_temp", static_cast<floating_t>(0));
  sink->addField("pdu_tof_bias_p5v0_vol", static_cast<floating_t>(0));
  sink->addField("pdu_tof_bias_p5v0_cur", static_cast<floating_t>(0));
  sink->addField("pdu_tof_bias_p5v1_vol", static_cast<floating_t>(0));
  sink->addField("pdu_tof_bias_p5v1_cur", static_cast<floating_t>(0));
  sink->addField("pdu_tof_bias_p5v_temp", static_cast<floating_t>(0));
  sink->addField("pdu_tof_p12v_vol", static_cast<floating_t>(0));
  sink->addField("pdu_tof_p12v_cur", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_p12v_vol", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_p12v_cur", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_m5v_vol", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_m5v_cur", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_p5v_vol", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_p5v_cur", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_p3v3_vol", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_p3v3_cur", static_cast<floating_t>(0));
  sink->addField("pdu_shaper_p3v3_vol", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_pm5v_temp", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_p12v_temp", static_cast<floating_t>(0));
  sink->addField("pdu_caen_nevis_p3v3_temp", static_cast<floating_t>(0));
  sink->addField("pdu_shaper_temp", static_cast<floating_t>(0));
  sink->addField("pdu_shaper_m3v3_vol", static_cast<floating_t>(0));
  for (size_t i = 0; i < NUM_PDU_WARM_TPC_SHAPER; ++i) {
    sink->addField("pdu_shaper_p_cur_" + std::to_string(i), static_cast<floating_t>(0));
    sink->addField("pdu_shaper_m_cur_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_PDU_CPU; ++i) {
    sink->addField("pdu_cpu_cur_" + std::to_string(i), static_cast<floating_t>(0));
    sink->addField("pdu_cpu_vol_" + std::to_string(i), static_cast<floating_t>(0));
  }
  sink->addField("pdu_cpu_unused_cur", static_cast<floating_t>(0));
  sink->addField("pressure_regulator_vol", static_cast<floating_t>(0));
  sink->addField("pdu_tof_p12v_temp", static_cast<floating_t>(0));
  sink->addField("pdu_main_bat_temp", static_cast<floating_t>(0));
  sink->addField("pdu_main_bat_cur", static_cast<floating_t>(0));
  sink->addField("pdu_main_bat_vol", static_cast<floating_t>(0));

  // MHADC
  for (size_t i = 0; i < NUM_RTD_GONDOLA; ++i) {
    sink->addField("rtd_gondola_frame_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_DAQ_CRATE; ++i) {
    sink->addField("rtd_daq_crate_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_SHAPER_FARADAY_CAGE; ++i) {
    sink->addField("rtd_shaper_faraday_cage_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_SHAPER_BOARD; ++i) {
    sink->addField("rtd_shaper_board_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_HUB_COMPUTER_LOCATION; ++i) {
    sink->addField("rtd_hub_computer_location_" + std::to_string(i), static_cast<floating_t>(0));
  }
  sink->addField("rtd_tof_fpgas", static_cast<floating_t>(0));
  sink->addField("rtd_tof", static_cast<floating_t>(0));
  for (size_t i = 0; i < NUM_RTD_OUTSIDE_SEALED_ENCLOSURE; ++i) {
    sink->addField("rtd_outside_sealed_enclosure_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_VACUUM_JACKET; ++i) {
    sink->addField("rtd_vacuum_jacket_" + std::to_string(i), static_cast<floating_t>(0));
  }
  sink->addField("pressure_transducer", static_cast<floating_t>(0));
  for (size_t i = 0; i < NUM_INCLINOMETERS; ++i) {
    sink->addField("inclinometer_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_RTD_INSIDE_CHAMBER; ++i) {
    sink->addField("rtd_inside_chamber_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_ADC_SPARE; ++i) {
    sink->addField("spare_" + std::to_string(i), static_cast<floating_t>(0));
  }
  sink->addField("sealed_enclosure_pressure", static_cast<floating_t>(0));
  sink->addField("sealed_enclosure_temperature", static_cast<floating_t>(0));
  sink->addField("sealed_enclosure_humidity", static_cast<floating_t>(0));
  for (size_t i = 0; i < NUM_PRESSURE_SENSORS; ++i) {
    sink->addField("pressure_sensor_" + std::to_string(i), static_cast<floating_t>(0));
  }
  sink->addField("lab_jack_temperature", static_cast<floating_t>(0));
  for (size_t i = 0; i < NUM_4_WIRE_RTD; ++i) {
    sink->addField("rtd_4_wire_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_TOF_BIAS; ++i) {
    sink->addField("tof_bias_voltage_" + std::to_string(i), static_cast<floating_t>(0));
    sink->addField("tof_bias_temperature_" + std::to_string(i), static_cast<floating_t>(0));
  }
  for (size_t i = 0; i < NUM_ERROR_FLAGS; ++i) {
    sink->addField("error_flag_" + std::to_string(i), static_cast<uint32_t>(0));
  }
  sink->addField("storage_size", static_cast<floating_t>(0));
  sink->addField("cpu_temperature", static_cast<floating_t>(0));
  sink->addField("ram_usage", static_cast<floating_t>(0));
}
} // namespace gramsballoon::pgrams