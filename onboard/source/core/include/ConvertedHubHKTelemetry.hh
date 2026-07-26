#ifndef GB_ConvertedHubHKTelemetry_hh
#define GB_ConvertedHubHKTelemetry_hh 1
#include "DBSerializable.hh"
#include "HubHKTelemetry.hh"

#define GETTER_ARRAY_F(name, variable, num)                                   \
  inline const std::array<floating_t, num> &name() const { return variable; } \
  inline floating_t name(size_t idx) const {                                  \
    if (idx >= num) {                                                         \
      std::cerr << #name << ": index out of range: " << idx << std::endl;     \
      return 0;                                                               \
    }                                                                         \
    return variable[idx];                                                     \
  }                                                                           \
  template <size_t N>                                                         \
  inline floating_t name() const {                                            \
    return std::get<N>(variable);                                             \
  }

namespace gramsballoon::pgrams {

/**
 * @brief Housekeeping Telemetry Definition for converted value
 * @author Shota Arai
 * @date 2026-06-16 | Shota Arai | First version
 */
class ConvertedHubHKTelemetry: public DBSerializable {
public:
  using floating_t = double;

public:
  static constexpr size_t NUM_TOF_BIAS = HubHKTelemetry::NUM_TOF_BIAS;
  static constexpr size_t NUM_PDU_HV_TEMP = HubHKTelemetry::NUM_PDU_HV_TEMP;
  static constexpr size_t NUM_PDU_SIPM = HubHKTelemetry::NUM_PDU_SIPM;
  static constexpr size_t ARGC = HubHKTelemetry::ARGC;
  static constexpr size_t NUM_PDU_WARM_TPC_SHAPER = HubHKTelemetry::NUM_PDU_WARM_TPC_SHAPER;
  static constexpr size_t NUM_ERROR_FLAGS = HubHKTelemetry::NUM_ERROR_FLAGS;
  static constexpr size_t NUM_RTD_GONDOLA = HubHKTelemetry::NUM_RTD_GONDOLA;
  static constexpr size_t NUM_RTD_SHAPER_BOARD = HubHKTelemetry::NUM_RTD_SHAPER_BOARD;
  static constexpr size_t NUM_PDU_CPU = HubHKTelemetry::NUM_PDU_CPU;
  static constexpr size_t NUM_PDU_TOF_TELEMETRY = HubHKTelemetry::NUM_PDU_TOF_TELEMETRY;
  static constexpr size_t NUM_4_WIRE_RTD = HubHKTelemetry::NUM_4_WIRE_RTD;
  static constexpr size_t NUM_RTD_VACUUM_JACKET = HubHKTelemetry::NUM_RTD_VACUUM_JACKET;
  static constexpr size_t NUM_RTD_SHAPER_FARADAY_CAGE = HubHKTelemetry::NUM_RTD_SHAPER_FARADAY_CAGE;
  static constexpr size_t NUM_RTD_DAQ_CRATE = HubHKTelemetry::NUM_RTD_DAQ_CRATE;
  static constexpr size_t NUM_RTD_HUB_COMPUTER_LOCATION = HubHKTelemetry::NUM_RTD_HUB_COMPUTER_LOCATION;
  static constexpr size_t NUM_RTD_OUTSIDE_SEALED_ENCLOSURE = HubHKTelemetry::NUM_RTD_OUTSIDE_SEALED_ENCLOSURE;
  static constexpr size_t NUM_RTD_INSIDE_CHAMBER = HubHKTelemetry::NUM_RTD_INSIDE_CHAMBER;
  static constexpr size_t NUM_ADC_SPARE = HubHKTelemetry::NUM_ADC_SPARE;
  static constexpr size_t NUM_PRESSURE_SENSORS = HubHKTelemetry::NUM_PRESSURE_SENSORS;
  static constexpr size_t NUM_INCLINOMETERS = HubHKTelemetry::NUM_INCLINOMETERS;
  static constexpr size_t ARG_INDEX_TOF_BIAS = HubHKTelemetry::ARG_INDEX_TOF_BIAS;

  ConvertedHubHKTelemetry();
  virtual ~ConvertedHubHKTelemetry();
  bool convert(const HubHKTelemetry *raw_telemetry);

  std::ostream &print(std::ostream &stream);

  void initializeDBTable(DBFieldSink *sink, const std::string &table_name) const override;
  void serialize(DBFieldSink *sink) const override;

  // Getters and setters
  inline uint16_t LastCommandCodeHub() const { return lastCommandCodeHub_; }
  inline uint32_t LastCommandIndexHub() const { return lastCommandIndexHub_; }
  inline uint32_t CommandRejectedIndexHub() const { return commandRejectedIndexHub_; }

  inline uint16_t LastCommandCodeOrc() const { return lastCommandCodeOrc_; }
  inline uint32_t LastCommandIndexOrc() const { return lastCommandIndexOrc_; }
  inline uint32_t CommandRejectedIndexOrc() const { return commandRejectedIndexOrc_; }

  inline uint16_t LastCommandCodeTPC() const { return lastCommandCodeTPC_; }
  inline uint32_t LastCommandIndexTPC() const { return lastCommandIndexTPC_; }
  inline uint32_t CommandRejectedIndexTPC() const { return commandRejectedIndexTPC_; }

  inline uint16_t LastCommandCodeTOF() const { return lastCommandCodeTOF_; }
  inline uint32_t LastCommandIndexTOF() const { return lastCommandIndexTOF_; }
  inline uint32_t CommandRejectedIndexTOF() const { return commandRejectedIndexTOF_; }

  inline uint16_t LastCommandCodeQM() const { return lastCommandCodeQM_; }
  inline uint32_t LastCommandIndexQM() const { return lastCommandIndexQM_; }
  inline uint32_t CommandRejectedIndexQM() const { return commandRejectedIndexQM_; }

  // PDU
  GETTER_ARRAY_F(PduVolSiPM, pduVolSiPM_, NUM_PDU_SIPM)
  GETTER_ARRAY_F(PduCurSiPM, pduCurSiPM_, NUM_PDU_SIPM)

  inline floating_t PduCurTPCHV() const { return pduCurTPCHV_; }
  inline floating_t PduVolTPCHV() const { return pduVolTPCHV_; }

  GETTER_ARRAY_F(PduHVTemp, pduHVTemp_, NUM_PDU_HV_TEMP)

  inline floating_t PduCommsBoardTemp() const { return pduCommsBoardTemp_; }
  inline floating_t PduSiPMPreAmpP2V5Vol() const { return pduSiPMPreAmpP2V5Vol_; }
  inline floating_t PduSiPMPreAmpP2V5Cur() const { return pduSiPMPreAmpP2V5Cur_; }
  inline floating_t PduSiPMPreAmpM5VVol() const { return pduSiPMPreAmpM5VVol_; }
  inline floating_t PduSiPMPreAmpM5VCur() const { return pduSiPMPreAmpM5VCur_; }
  inline floating_t PduSiPMPreAmpTemp() const { return pduSiPMPreAmpTemp_; }
  inline floating_t PduChargePreAmpP5VVol() const { return pduChargePreAmpP5VVol_; }
  inline floating_t PduChargePreAmpP5VCur() const { return pduChargePreAmpP5VCur_; }
  inline floating_t PduChargePreAmpM5VVol() const { return pduChargePreAmpM5VVol_; }
  inline floating_t PduChargePreAmpM5VCur() const { return pduChargePreAmpM5VCur_; }
  inline floating_t PduChargePreAmpTemp() const { return pduChargePreAmpTemp_; }
  inline floating_t PduTofBiasP5V0Vol() const { return pduTofBiasP5V0Vol_; }
  inline floating_t PduTofBiasP5V0Cur() const { return pduTofBiasP5V0Cur_; }
  inline floating_t PduTofBiasP5V1Vol() const { return pduTofBiasP5V1Vol_; }
  inline floating_t PduTofBiasP5V1Cur() const { return pduTofBiasP5V1Cur_; }
  inline floating_t PduTofBiasP5VTemp() const { return pduTofBiasP5VTemp_; }
  inline floating_t PduTofP12VVol() const { return pduTofP12VVol_; }
  inline floating_t PduTofP12VCur() const { return pduTofP12VCur_; }
  inline floating_t PduCaenNevisP12VVol() const { return pduCaenNevisP12VVol_; }
  inline floating_t PduCaenNevisP12VCur() const { return pduCaenNevisP12VCur_; }
  inline floating_t PduCaenNevisM5VVol() const { return pduCaenNevisM5VVol_; }
  inline floating_t PduCaenNevisM5VCur() const { return pduCaenNevisM5VCur_; }
  inline floating_t PduCaenNevisP5VCur() const { return pduCaenNevisP5VCur_; }
  inline floating_t PduCaenNevisP5VVol() const { return pduCaenNevisP5VVol_; }
  inline floating_t PduCaenNevisP3V3Cur() const { return pduCaenNevisP3V3Cur_; }
  inline floating_t PduCaenNevisP3V3Vol() const { return pduCaenNevisP3V3Vol_; }
  inline floating_t PduShaperP3V3Vol() const { return pduShaperP3V3Vol_; }
  inline floating_t PduCaenNevisPM5VTemp() const { return pduCaenNevisPM5VTemp_; }
  inline floating_t PduCaenNevisP12VTemp() const { return pduCaenNevisP12VTemp_; }
  inline floating_t PduCaenNevisP3V3Temp() const { return pduCaenNevisP3V3Temp_; }
  inline floating_t PduShaperTemp() const { return pduShaperTemp_; }
  inline floating_t PduShaperM3V3Vol() const { return pduShaperM3V3Vol_; }

  GETTER_ARRAY_F(PduShaperPCur, pduShaperPCur_, NUM_PDU_WARM_TPC_SHAPER)
  GETTER_ARRAY_F(PduShaperMCur, pduShaperMCur_, NUM_PDU_WARM_TPC_SHAPER)
  GETTER_ARRAY_F(PduCPUCur, pduCPUCur_, NUM_PDU_CPU)

  inline floating_t PduCpuUnusedCur() const { return pduCpuUnusedCur_; }

  GETTER_ARRAY_F(PduCPUVol, pduCPUVol_, NUM_PDU_CPU)

  inline floating_t PressureRegulatorVol() const { return pressureRegulatorVol_; }
  inline floating_t PduTofP12VTemp() const { return pduTofP12VTemp_; }
  inline floating_t PduCurMainBat() const { return pduCurMainBat_; }
  inline floating_t PduVolMainBat() const { return pduVolMainBat_; }
  inline floating_t PduMainBatTemp() const { return pduMainBatTemp_; }

  // MHADC
  GETTER_ARRAY_F(RtdGondolaFrame, rtdGondolaFrame_, NUM_RTD_GONDOLA)
  GETTER_ARRAY_F(RtdDaqCrate, rtdDaqCrate_, NUM_RTD_DAQ_CRATE)
  GETTER_ARRAY_F(RtdShaperFaradayCage, rtdShaperFaradayCage_, NUM_RTD_SHAPER_FARADAY_CAGE)
  GETTER_ARRAY_F(RtdShaperBoard, rtdShaperBoard_, NUM_RTD_SHAPER_BOARD)
  GETTER_ARRAY_F(RtdHubComputerLocation, rtdHubComputerLocation_, NUM_RTD_HUB_COMPUTER_LOCATION)

  inline floating_t RtdTofFpga() const { return rtdTofFpga_; }
  inline floating_t RtdTof() const { return rtdTof_; }

  GETTER_ARRAY_F(RtdOutsideSealedEnclosure, rtdOutsideSealedEnclosure_, NUM_RTD_OUTSIDE_SEALED_ENCLOSURE)
  GETTER_ARRAY_F(RtdVacuumJacket, rtdVacuumJacket_, NUM_RTD_VACUUM_JACKET)

  inline floating_t PressureTransducer() const { return pressureTransducer_; }

  GETTER_ARRAY_F(Inclinometers, inclinometers_, NUM_INCLINOMETERS)
  GETTER_ARRAY_F(RtdsInsideChamber, rtdsInsideChamber_, NUM_RTD_INSIDE_CHAMBER)
  GETTER_ARRAY_F(Spare, spare_, NUM_ADC_SPARE)

  inline floating_t SealedEnclosurePressure() const { return sealedEnclosurePressure_; }
  inline floating_t SealedEnclosureTemperature() const { return sealedEnclosureTemperature_; }
  inline floating_t SealedEnclosureHumidity() const { return sealedEnclosureHumidity_; }

  GETTER_ARRAY_F(PressureSensors, pressureSensors_, NUM_PRESSURE_SENSORS)

  inline floating_t LabJackTemperature() const { return labJackTemperature_; }

  GETTER_ARRAY_F(Rtd4Wire, rtd4Wire_, NUM_4_WIRE_RTD)
  GETTER_ARRAY_F(TofBiasVoltage, tofBiasVoltage_, NUM_TOF_BIAS)
  GETTER_ARRAY_F(TofBiasTemperature, tofBiasTemperature_, NUM_TOF_BIAS)

  inline const std::array<uint32_t, NUM_ERROR_FLAGS> &HubComputerErrorFlags() const { return hubComputerErrorFlags_; }
  inline uint32_t HubComputerErrorFlags(size_t idx) const { return hubComputerErrorFlags_[idx]; }

  inline floating_t StorageSize() const { return storageSize_; }
  inline floating_t CpuTemperature() const { return cpuTemperature_; }
  inline floating_t RamUsage() const { return ramUsage_; }

#undef GETTER_ARRAY_F

private:
  std::time_t timeStamp_ = 0;
  uint32_t index_ = 0;
  uint32_t runid_ = 0; // 0 means unset
  Subsystem subsystem_ = Subsystem::UNKNOWN;
  uint16_t code_ = 0;

  uint16_t lastCommandCodeHub_ = 0;
  uint32_t lastCommandIndexHub_ = 0;
  uint32_t commandRejectedIndexHub_ = 0;
  uint16_t lastCommandCodeOrc_ = 0;
  uint32_t lastCommandIndexOrc_ = 0;
  uint32_t commandRejectedIndexOrc_ = 0;
  uint16_t lastCommandCodeTPC_ = 0;
  uint32_t lastCommandIndexTPC_ = 0;
  uint32_t commandRejectedIndexTPC_ = 0;
  uint16_t lastCommandCodeTOF_ = 0;
  uint32_t lastCommandIndexTOF_ = 0;
  uint32_t commandRejectedIndexTOF_ = 0;
  uint16_t lastCommandCodeQM_ = 0;
  uint32_t lastCommandIndexQM_ = 0;
  uint32_t commandRejectedIndexQM_ = 0;

  // PDU
  std::array<floating_t, NUM_PDU_SIPM> pduVolSiPM_ = {0};
  std::array<floating_t, NUM_PDU_SIPM> pduCurSiPM_ = {0};
  floating_t pduCurTPCHV_ = 0;
  floating_t pduVolTPCHV_ = 0;
  std::array<floating_t, NUM_PDU_HV_TEMP> pduHVTemp_ = {0};
  floating_t pduCommsBoardTemp_ = 0;
  floating_t pduSiPMPreAmpP2V5Vol_ = 0;
  floating_t pduSiPMPreAmpP2V5Cur_ = 0;
  floating_t pduSiPMPreAmpM5VVol_ = 0;
  floating_t pduSiPMPreAmpM5VCur_ = 0;
  floating_t pduSiPMPreAmpTemp_ = 0;
  floating_t pduChargePreAmpP5VVol_ = 0;
  floating_t pduChargePreAmpP5VCur_ = 0;
  floating_t pduChargePreAmpM5VVol_ = 0;
  floating_t pduChargePreAmpM5VCur_ = 0;
  floating_t pduChargePreAmpTemp_ = 0;
  floating_t pduTofBiasP5V0Vol_ = 0;
  floating_t pduTofBiasP5V0Cur_ = 0;
  floating_t pduTofBiasP5V1Vol_ = 0;
  floating_t pduTofBiasP5V1Cur_ = 0;
  floating_t pduTofBiasP5VTemp_ = 0;
  floating_t pduTofP12VCur_ = 0;
  floating_t pduTofP12VVol_ = 0;
  floating_t pduCaenNevisP12VVol_ = 0;
  floating_t pduCaenNevisP12VCur_ = 0;
  floating_t pduCaenNevisM5VVol_ = 0;
  floating_t pduCaenNevisM5VCur_ = 0;
  floating_t pduCaenNevisP5VCur_ = 0;
  floating_t pduCaenNevisP5VVol_ = 0;
  floating_t pduCaenNevisP3V3Cur_ = 0;
  floating_t pduCaenNevisP3V3Vol_ = 0;
  floating_t pduShaperP3V3Vol_ = 0;
  floating_t pduCaenNevisPM5VTemp_ = 0;
  floating_t pduCaenNevisP12VTemp_ = 0;
  floating_t pduCaenNevisP3V3Temp_ = 0;
  floating_t pduShaperTemp_ = 0;
  floating_t pduShaperM3V3Vol_ = 0;
  std::array<floating_t, NUM_PDU_WARM_TPC_SHAPER> pduShaperPCur_ = {0};
  std::array<floating_t, NUM_PDU_WARM_TPC_SHAPER> pduShaperMCur_ = {0};
  std::array<floating_t, NUM_PDU_CPU> pduCPUCur_ = {0};
  floating_t pduCpuUnusedCur_ = 0;
  std::array<floating_t, NUM_PDU_CPU> pduCPUVol_ = {0};
  floating_t pressureRegulatorVol_ = 0;
  floating_t pduTofP12VTemp_ = 0;
  floating_t pduCurMainBat_ = 0;
  floating_t pduVolMainBat_ = 0;
  floating_t pduMainBatTemp_ = 0;

  // MHADC
  std::array<floating_t, NUM_RTD_GONDOLA> rtdGondolaFrame_ = {0};
  std::array<floating_t, NUM_RTD_DAQ_CRATE> rtdDaqCrate_ = {0};
  std::array<floating_t, NUM_RTD_SHAPER_FARADAY_CAGE> rtdShaperFaradayCage_ = {0};
  std::array<floating_t, NUM_RTD_SHAPER_BOARD> rtdShaperBoard_ = {0};
  std::array<floating_t, NUM_RTD_HUB_COMPUTER_LOCATION> rtdHubComputerLocation_ = {0};
  floating_t rtdTofFpga_ = 0;
  floating_t rtdTof_ = 0;
  std::array<floating_t, NUM_RTD_OUTSIDE_SEALED_ENCLOSURE> rtdOutsideSealedEnclosure_ = {0};
  std::array<floating_t, NUM_RTD_VACUUM_JACKET> rtdVacuumJacket_ = {0};
  floating_t pressureTransducer_ = 0;
  std::array<floating_t, NUM_INCLINOMETERS> inclinometers_ = {0};
  std::array<floating_t, NUM_RTD_INSIDE_CHAMBER> rtdsInsideChamber_ = {0};
  std::array<floating_t, NUM_ADC_SPARE> spare_ = {0};

  floating_t sealedEnclosurePressure_ = 0;
  floating_t sealedEnclosureTemperature_ = 0;
  floating_t sealedEnclosureHumidity_ = 0;
  std::array<floating_t, NUM_PRESSURE_SENSORS> pressureSensors_ = {0};
  floating_t labJackTemperature_ = 0;
  std::array<floating_t, NUM_4_WIRE_RTD> rtd4Wire_ = {0};

  //Tof bias
  std::array<floating_t, NUM_TOF_BIAS> tofBiasVoltage_ = {0};
  std::array<floating_t, NUM_TOF_BIAS> tofBiasTemperature_ = {0};

  //Hub computer
  std::array<uint32_t, NUM_ERROR_FLAGS> hubComputerErrorFlags_ = {0};
  floating_t storageSize_ = 0;
  floating_t cpuTemperature_ = 0;
  floating_t ramUsage_ = 0;

  template <typename T>
  static bool convertVoltageMHADC(T adc_value, floating_t &src);
  template <typename T>
  static bool convertRTD(T adc_value, floating_t &src, floating_t offset);
  template <typename T>
  static bool convertInclinometer(T adc_value, floating_t &src, floating_t offset);
  template <typename T>
  static bool convertPDUSiPMVoltage(T adc_value, floating_t &src, floating_t offset);
  template <typename T>
  static bool convertPDUSiPMCurrent(T adc_value, floating_t &src, floating_t offset);
  template <typename T>
  static bool convertPDUTPCHVCurrent(T adc_value, floating_t &src, floating_t offset);
  template <typename T>
  static bool convertVoltagePDU(T adc_value, floating_t &src);
  template <typename T>
  static bool convertBME680Temp(T value, floating_t &temp_dest, floating_t);
  template <typename T>
  static bool convertBME680Press(T value, floating_t &press_dest, floating_t);
  template <typename T>
  static bool convertBME680Humid(T value, floating_t &humid_dest, floating_t);
  template <typename T>
  static bool convertLabJackTemp(T value, floating_t &labjack_temp, floating_t);
  template <typename T>
  static bool convertPressTransducer(T value, floating_t &pressTransducer, floating_t);
};

} // namespace gramsballoon::pgrams
#endif //GB_ConvertedHubHKTelemetry_hh