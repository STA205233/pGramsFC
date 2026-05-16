#ifndef GRAMSBalloon_HubHKTelemetry_HH
#define GRAMSBalloon_HubHKTelemetry_HH 1
#include "BaseTelemetryDefinition.hh"
#include "ErrorManager.hh"

#define GETTER_SETTER_ARRAY(type, name, variable, num)                             \
  inline void set##name(const std::array<type, num> &v) { variable = v; }          \
  inline void set##name(size_t idx, type v) {                                      \
    if (idx >= num) {                                                              \
      std::cerr << "set" << #name << ": index out of range: " << idx << std::endl; \
      return;                                                                      \
    }                                                                              \
    variable[idx] = v;                                                             \
  }                                                                                \
  inline const std::array<type, num> &name() const { return variable; }            \
  inline type name(size_t idx) const {                                             \
    if (idx >= num) {                                                              \
      std::cerr << #name << ": index out of range: " << idx << std::endl;          \
      return 0;                                                                    \
    }                                                                              \
    return variable[idx];                                                          \
  }
namespace gramsballoon::pgrams {

/**
 * @brief Hub Housekeeping Telemetry Definition
 * @author Shota Arai
 * @date 2025-11-** | Shota Arai | First design
 * @date 2025-12-15 | Shota Arai | Added serialize and initializeDBTable methods
 * @date 2026-01-22 | Shota Arai | Introduce command rejected index and remove TOF bias setting
 */
class HubHKTelemetry: public BaseTelemetryDefinition {
public:
  HubHKTelemetry(bool instantiateContents) : BaseTelemetryDefinition(instantiateContents) {
    setType(Subsystem::HUB);
  }
  virtual ~HubHKTelemetry() = default;
  static constexpr size_t NUM_TOF_BIAS = 200;
  static constexpr size_t NUM_PDU_HV_TEMP = 2;
  static constexpr size_t NUM_PDU_SIPM = 6;
  static constexpr size_t ARGC = 1148 / 4;
  static constexpr size_t NUM_PDU_WARM_TPC_SHAPER = 6;
  static constexpr size_t NUM_ERROR_FLAGS = ErrorManager::NUM_ERROR_FLAGS;
  static constexpr size_t NUM_RTD_GONDOLA = 4;
  static constexpr size_t NUM_RTD_SHAPER_BOARD = 6;
  static constexpr size_t NUM_PDU_CPU = 7;
  static constexpr size_t NUM_PDU_TOF_TELEMETRY = 6;
  static constexpr size_t NUM_SPARE = 11;
  static constexpr size_t NUM_4_WIRE_RTD = 2;
  static constexpr size_t NUM_RTD_VACUUM_JACKET = 3;
  static constexpr size_t NUM_RTD_SHAPER_FARADAY_CAGE = 2;
  static constexpr size_t NUM_RTD_DAQ_CRATE = 3;
  static constexpr size_t NUM_RTD_HUB_COMPUTER_LOCATION = 2;
  static constexpr size_t NUM_RTD_OUTSIDE_SEALED_ENCLOSURE = 2;
  static constexpr size_t NUM_RTD_INSIDE_CHAMBER = 9;
  static constexpr size_t NUM_SPARE = 13;
  static constexpr size_t NUM_PRESSURE_SENSORS = 2;
  static constexpr size_t NUM_INCLINOMETERS = 2;

public:
  void
  serialize(DBFieldSink *sink) const override;
  void initializeDBTable(DBFieldSink *sink, const std::string &table_name) const override;

private:
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
  std::array<uint16_t, NUM_PDU_SIPM> pduVolSiPM_ = {0};
  std::array<uint16_t, NUM_PDU_SIPM> pduCurSiPM_ = {0};
  uint16_t pduCurTPCHV_ = 0;
  uint16_t pduVolTPCHV_ = 0;
  std::array<uint16_t, NUM_PDU_HV_TEMP> pduHVTemp_ = {0};
  uint16_t pduCommsBoardTemp_ = 0;
  uint16_t pduSiPMPreAmpP2V5Vol_ = 0;
  uint16_t pduSiPMPreAmpP2V5Cur_ = 0;
  uint16_t pduSiPMPreAmpM5VVol_ = 0;
  uint16_t pduSiPMPreAmpM5VCur_ = 0;
  uint16_t pduSiPMPreAmpTemp_ = 0;
  uint16_t pduChargePreAmpP5VVol_ = 0;
  uint16_t pduChargePreAmpP5VCur_ = 0;
  uint16_t pduChargePreAmpM5VVol_ = 0;
  uint16_t pduChargePreAmpM5VCur_ = 0;
  uint16_t pduChargePreAmpTemp_ = 0;
  uint16_t pduTofBiasP5V0Vol_ = 0;
  uint16_t pduTofBiasP5V0Cur_ = 0;
  uint16_t pduTofBiasP5V1Vol_ = 0;
  uint16_t pduTofBiasP5V1Cur_ = 0;
  uint16_t pduTofBiasP5VTemp_ = 0;
  uint16_t pduTofP12VCur_ = 0;
  uint16_t pduTofP12VVol_ = 0;
  uint16_t pduCaenNevisP12VVol_ = 0;
  uint16_t pduCaenNevisP12VCur_ = 0;
  uint16_t pduCaenNevisM5VVol_ = 0;
  uint16_t pduCaenNevisM5VCur_ = 0;
  uint16_t pduCaenNevisP5VCur_ = 0;
  uint16_t pduCaenNevisP5VVol_ = 0;
  uint16_t pduCaenNevisP3V3Cur_ = 0;
  uint16_t pduCaenNevisP3V3Vol_ = 0;
  uint16_t pduShaperP3V3Vol_ = 0;
  uint16_t pduCaenNevisPM5VTemp_ = 0;
  uint16_t pduCaenNevisP12VTemp_ = 0;
  uint16_t pduCaenNevisP3V3Temp_ = 0;
  uint16_t pduShaperTemp_ = 0;
  uint16_t pduShaperM3V3Vol_ = 0;
  std::array<uint16_t, NUM_PDU_WARM_TPC_SHAPER> pduShaperPCur_ = {0};
  std::array<uint16_t, NUM_PDU_WARM_TPC_SHAPER> pduShaperMCur_ = {0};
  std::array<uint16_t, NUM_PDU_CPU> pduCPUCur_ = {0};
  uint16_t pduCpuUnusedCur_ = 0;
  std::array<uint16_t, NUM_PDU_CPU> pduCPUVol_ = {0};
  uint16_t pressureRegulatorVol_ = 0;
  uint16_t pduTofP12VTemp_ = 0;
  uint16_t pduCurMainBat_ = 0;
  uint16_t pduVolMainBat_ = 0;
  uint16_t pduMainBatTemp_ = 0;

  // MHADC
  std::array<uint16_t, NUM_RTD_GONDOLA> rtdGondolaFrame_ = {0};
  std::array<uint16_t, NUM_RTD_DAQ_CRATE> rtdDaqCrate_ = {0};
  std::array<uint16_t, NUM_RTD_SHAPER_FARADAY_CAGE> rtdShaperFaradayCage_ = {0};
  std::array<uint16_t, NUM_RTD_SHAPER_BOARD> rtdShaperBoard_ = {0};
  std::array<uint16_t, NUM_RTD_HUB_COMPUTER_LOCATION> rtdHubComputerLocation_ = {0};
  uint16_t rtdTofFpga_ = 0;
  uint16_t rtdTof_ = 0;
  std::array<uint16_t, NUM_RTD_OUTSIDE_SEALED_ENCLOSURE> rtdOutsideSealedEnclosure_ = {0};
  std::array<uint16_t, NUM_RTD_VACUUM_JACKET> rtdVacuumJacket_ = {0};
  uint16_t pressureTransducer_ = 0;
  std::array<uint16_t, NUM_INCLINOMETERS> inclinometers_ = {0};
  std::array<uint16_t, NUM_RTD_INSIDE_CHAMBER> rtdsInsideChamber_ = {0};
  std::array<uint16_t, NUM_SPARE> spare_ = {0};

  uint16_t sealedEnclosurePressure_ = 0;
  uint16_t sealedEnclosureTemperature_ = 0;
  uint16_t sealedEnclosureHumidity_ = 0;
  std::array<uint16_t, NUM_PRESSURE_SENSORS> pressureSensors_ = {0};
  std::array<uint16_t, NUM_4_WIRE_RTD> rtd4Wire_ = {0};

  //Tof bias
  std::array<uint16_t, NUM_TOF_BIAS> tofBiasVoltage_ = {0};
  std::array<uint16_t, NUM_TOF_BIAS> tofBiasTemperature_ = {0};

  //Hub computer
  std::array<uint32_t, NUM_ERROR_FLAGS> hubComputerErrorFlags_ = {0};
  uint32_t storageSize_ = 0;
  uint16_t cpuTemperature_ = 0;
  uint16_t ramUsage_ = 0;

protected:
  bool interpret() override;

public:
  void update() override;
  std::ostream &print(std::ostream &stream) override;

  // Getters and setters

  inline void setLastCommandCodeHub(uint16_t v) { lastCommandCodeHub_ = v; }
  inline uint16_t LastCommandCodeHub() const { return lastCommandCodeHub_; }

  inline void setLastCommandIndexHub(uint32_t v) { lastCommandIndexHub_ = v; }
  inline uint32_t LastCommandIndexHub() const { return lastCommandIndexHub_; }

  inline void setCommandRejectedIndexHub(uint32_t v) { commandRejectedIndexHub_ = v; }
  inline uint32_t CommandRejectedIndexHub() const { return commandRejectedIndexHub_; }

  inline void setLastCommandCodeOrc(uint16_t v) { lastCommandCodeOrc_ = v; }
  inline uint16_t LastCommandCodeOrc() const { return lastCommandCodeOrc_; }

  inline void setLastCommandIndexOrc(uint32_t v) { lastCommandIndexOrc_ = v; }
  inline uint32_t LastCommandIndexOrc() const { return lastCommandIndexOrc_; }

  inline void setCommandRejectedIndexOrc(uint32_t v) { commandRejectedIndexOrc_ = v; }
  inline uint32_t CommandRejectedIndexOrc() const { return commandRejectedIndexOrc_; }

  inline void setLastCommandCodeTPC(uint16_t v) { lastCommandCodeTPC_ = v; }
  inline uint16_t LastCommandCodeTPC() const { return lastCommandCodeTPC_; }

  inline void setLastCommandIndexTPC(uint32_t v) { lastCommandIndexTPC_ = v; }
  inline uint32_t LastCommandIndexTPC() const { return lastCommandIndexTPC_; }

  inline void setCommandRejectedIndexTPC(uint32_t v) { commandRejectedIndexTPC_ = v; }
  inline uint32_t CommandRejectedIndexTPC() const { return commandRejectedIndexTPC_; }

  inline void setLastCommandCodeTOF(uint16_t v) { lastCommandCodeTOF_ = v; }
  inline uint16_t LastCommandCodeTOF() const { return lastCommandCodeTOF_; }

  inline void setLastCommandIndexTOF(uint32_t v) { lastCommandIndexTOF_ = v; }
  inline uint32_t LastCommandIndexTOF() const { return lastCommandIndexTOF_; }

  inline void setCommandRejectedIndexTOF(uint32_t v) { commandRejectedIndexTOF_ = v; }
  inline uint32_t CommandRejectedIndexTOF() const { return commandRejectedIndexTOF_; }

  inline void setLastCommandCodeQM(uint16_t v) { lastCommandCodeQM_ = v; }
  inline uint16_t LastCommandCodeQM() const { return lastCommandCodeQM_; }

  inline void setLastCommandIndexQM(uint32_t v) { lastCommandIndexQM_ = v; }
  inline uint32_t LastCommandIndexQM() const { return lastCommandIndexQM_; }

  inline void setCommandRejectedIndexQM(uint32_t v) { commandRejectedIndexQM_ = v; }
  inline uint32_t CommandRejectedIndexQM() const { return commandRejectedIndexQM_; }

  // PDU
  GETTER_SETTER_ARRAY(uint16_t, PduVolSiPM, pduVolSiPM_, NUM_PDU_SIPM);
  GETTER_SETTER_ARRAY(uint16_t, PduCurSiPM, pduCurSiPM_, NUM_PDU_SIPM);

  inline void setPduCurTPCHV(uint16_t v) { pduCurTPCHV_ = v; }
  inline uint16_t PduCurTPCHV() const { return pduCurTPCHV_; }

  inline void setPduVolTPCHV(uint16_t v) { pduVolTPCHV_ = v; }
  inline uint16_t PduVolTPCHV() const { return pduVolTPCHV_; }

  GETTER_SETTER_ARRAY(uint16_t, PduHVTemp, pduHVTemp_, NUM_PDU_HV_TEMP);

  inline void setPduCommsBoardTemp(uint16_t v) { pduCommsBoardTemp_ = v; }
  inline uint16_t PduCommsBoardTemp() const { return pduCommsBoardTemp_; }

  inline void setPduSiPMPreAmpP2V5Vol(uint16_t v) { pduSiPMPreAmpP2V5Vol_ = v; }
  inline uint16_t PduSiPMPreAmpP2V5Vol() const { return pduSiPMPreAmpP2V5Vol_; }

  inline void setPduSiPMPreAmpP2V5Cur(uint16_t v) { pduSiPMPreAmpP2V5Cur_ = v; }
  inline uint16_t PduSiPMPreAmpP2V5Cur() const { return pduSiPMPreAmpP2V5Cur_; }

  inline void setPduSiPMPreAmpM5VVol(uint16_t v) { pduSiPMPreAmpM5VVol_ = v; }
  inline uint16_t PduSiPMPreAmpM5VVol() const { return pduSiPMPreAmpM5VVol_; }

  inline void setPduSiPMPreAmpM5VCur(uint16_t v) { pduSiPMPreAmpM5VCur_ = v; }
  inline uint16_t PduSiPMPreAmpM5VCur() const { return pduSiPMPreAmpM5VCur_; }

  inline void setPduSiPMPreAmpTemp(uint16_t v) { pduSiPMPreAmpTemp_ = v; }
  inline uint16_t PduSiPMPreAmpTemp() const { return pduSiPMPreAmpTemp_; }

  inline void setPduChargePreAmpP5VVol(uint16_t v) { pduChargePreAmpP5VVol_ = v; }
  inline uint16_t PduChargePreAmpP5VVol() const { return pduChargePreAmpP5VVol_; }

  inline void setPduChargePreAmpP5VCur(uint16_t v) { pduChargePreAmpP5VCur_ = v; }
  inline uint16_t PduChargePreAmpP5VCur() const { return pduChargePreAmpP5VCur_; }

  inline void setPduChargePreAmpM5VVol(uint16_t v) { pduChargePreAmpM5VVol_ = v; }
  inline uint16_t PduChargePreAmpM5VVol() const { return pduChargePreAmpM5VVol_; }

  inline void setPduChargePreAmpM5VCur(uint16_t v) { pduChargePreAmpM5VCur_ = v; }
  inline uint16_t PduChargePreAmpM5VCur() const { return pduChargePreAmpM5VCur_; }

  inline void setPduChargePreAmpTemp(uint16_t v) { pduChargePreAmpTemp_ = v; }
  inline uint16_t PduChargePreAmpTemp() const { return pduChargePreAmpTemp_; }

  inline void setPduTofBiasP5V0Vol(uint16_t v) { pduTofBiasP5V0Vol_ = v; }
  inline uint16_t PduTofBiasP5V0Vol() const { return pduTofBiasP5V0Vol_; }

  inline void setPduTofBiasP5V0Cur(uint16_t v) { pduTofBiasP5V0Cur_ = v; }
  inline uint16_t PduTofBiasP5V0Cur() const { return pduTofBiasP5V0Cur_; }

  inline void setPduTofBiasP5V1Vol(uint16_t v) { pduTofBiasP5V1Vol_ = v; }
  inline uint16_t PduTofBiasP5V1Vol() const { return pduTofBiasP5V1Vol_; }

  inline void setPduTofBiasP5V1Cur(uint16_t v) { pduTofBiasP5V1Cur_ = v; }
  inline uint16_t PduTofBiasP5V1Cur() const { return pduTofBiasP5V1Cur_; }

  inline void setPduTofBiasP5VTemp(uint16_t v) { pduTofBiasP5VTemp_ = v; }
  inline uint16_t PduTofBiasP5VTemp() const { return pduTofBiasP5VTemp_; }

  inline void setPduTofP12VVol(uint16_t v) { pduTofP12VVol_ = v; }
  inline uint16_t PduTofP12VVol() const { return pduTofP12VVol_; }

  inline void setPduTofP12VCur(uint16_t v) { pduTofP12VCur_ = v; }
  inline uint16_t PduTofP12VCur() const { return pduTofP12VCur_; }

  inline void setPduCaenNevisP12VVol(uint16_t v) { pduCaenNevisP12VVol_ = v; }
  inline uint16_t PduCaenNevisP12VVol() const { return pduCaenNevisP12VVol_; }

  inline void setPduCaenNevisP12VCur(uint16_t v) { pduCaenNevisP12VCur_ = v; }
  inline uint16_t PduCaenNevisP12VCur() const { return pduCaenNevisP12VCur_; }

  inline void setPduCaenNevisM5VVol(uint16_t v) { pduCaenNevisM5VVol_ = v; }
  inline uint16_t PduCaenNevisM5VVol() const { return pduCaenNevisM5VVol_; }

  inline void setPduCaenNevisM5VCur(uint16_t v) { pduCaenNevisM5VCur_ = v; }
  inline uint16_t PduCaenNevisM5VCur() const { return pduCaenNevisM5VCur_; }

  inline void setPduCaenNevisP5VCur(uint16_t v) { pduCaenNevisP5VCur_ = v; }
  inline uint16_t PduCaenNevisP5VCur() const { return pduCaenNevisP5VCur_; }

  inline void setPduCaenNevisP5VVol(uint16_t v) { pduCaenNevisP5VVol_ = v; }
  inline uint16_t PduCaenNevisP5VVol() const { return pduCaenNevisP5VVol_; }

  inline void setPduCaenNevisP3V3Cur(uint16_t v) { pduCaenNevisP3V3Cur_ = v; }
  inline uint16_t PduCaenNevisP3V3Cur() const { return pduCaenNevisP3V3Cur_; }

  inline void setPduCaenNevisP3V3Vol(uint16_t v) { pduCaenNevisP3V3Vol_ = v; }
  inline uint16_t PduCaenNevisP3V3Vol() const { return pduCaenNevisP3V3Vol_; }

  inline void setPduShaperP3V3Vol(uint16_t v) { pduShaperP3V3Vol_ = v; }
  inline uint16_t PduShaperP3V3Vol() const { return pduShaperP3V3Vol_; }

  inline void setPduCaenNevisPM5VTemp(uint16_t v) { pduCaenNevisPM5VTemp_ = v; }
  inline uint16_t PduCaenNevisPM5VTemp() const { return pduCaenNevisPM5VTemp_; }

  inline void setPduCaenNevisP12VTemp(uint16_t v) { pduCaenNevisP12VTemp_ = v; }
  inline uint16_t PduCaenNevisP12VTemp() const { return pduCaenNevisP12VTemp_; }

  inline void setPduCaenNevisP3V3Temp(uint16_t v) { pduCaenNevisP3V3Temp_ = v; }
  inline uint16_t PduCaenNevisP3V3Temp() const { return pduCaenNevisP3V3Temp_; }

  inline void setPduShaperTemp(uint16_t v) { pduShaperTemp_ = v; }
  inline uint16_t PduShaperTemp() const { return pduShaperTemp_; }

  inline void setPduShaperM3V3Vol(uint16_t v) { pduShaperM3V3Vol_ = v; }
  inline uint16_t PduShaperM3V3Vol() const { return pduShaperM3V3Vol_; }

  GETTER_SETTER_ARRAY(uint16_t, PduShaperPCur, pduShaperPCur_, NUM_PDU_WARM_TPC_SHAPER);
  GETTER_SETTER_ARRAY(uint16_t, PduShaperMCur, pduShaperMCur_, NUM_PDU_WARM_TPC_SHAPER);
  GETTER_SETTER_ARRAY(uint16_t, PduCPUCur, pduCPUCur_, NUM_PDU_CPU);

  inline void setPduCpuUnusedCur(uint16_t v) { pduCpuUnusedCur_ = v; }
  inline uint16_t PduCpuUnusedCur() const { return pduCpuUnusedCur_; }

  GETTER_SETTER_ARRAY(uint16_t, PduCPUVol, pduCPUVol_, NUM_PDU_CPU);

  inline void setPressureRegulatorVol(uint16_t v) { pressureRegulatorVol_ = v; }
  inline uint16_t PressureRegulatorVol() const { return pressureRegulatorVol_; }

  inline void setPduTofP12VTemp(uint16_t v) { pduTofP12VTemp_ = v; }
  inline uint16_t PduTofP12VTemp() const { return pduTofP12VTemp_; }

  inline void setPduCurMainBat(uint16_t v) { pduCurMainBat_ = v; }
  inline uint16_t PduCurMainBat() const { return pduCurMainBat_; }

  inline void setPduVolMainBat(uint16_t v) { pduVolMainBat_ = v; }
  inline uint16_t PduVolMainBat() const { return pduVolMainBat_; }

  inline void setPduMainBatTemp(uint16_t v) { pduMainBatTemp_ = v; }
  inline uint16_t PduMainBatTemp() const { return pduMainBatTemp_; }

  // MHADC
  GETTER_SETTER_ARRAY(uint16_t, RtdGondolaFrame, rtdGondolaFrame_, NUM_RTD_GONDOLA);
  GETTER_SETTER_ARRAY(uint16_t, RtdDaqCrate, rtdDaqCrate_, NUM_RTD_DAQ_CRATE);
  GETTER_SETTER_ARRAY(uint16_t, RtdShaperFaradayCage, rtdShaperFaradayCage_, NUM_RTD_SHAPER_FARADAY_CAGE);
  GETTER_SETTER_ARRAY(uint16_t, RtdShaperBoard, rtdShaperBoard_, NUM_RTD_SHAPER_BOARD);
  GETTER_SETTER_ARRAY(uint16_t, RtdHubComputerLocation, rtdHubComputerLocation_, NUM_RTD_HUB_COMPUTER_LOCATION);

  inline void setRtdTofFpga(uint16_t v) { rtdTofFpga_ = v; }
  inline uint16_t RtdTofFpga() const { return rtdTofFpga_; }

  inline void setRtdTof(uint16_t v) { rtdTof_ = v; }
  inline uint16_t RtdTof() const { return rtdTof_; }

  GETTER_SETTER_ARRAY(uint16_t, RtdOutsideSealedEnclosure, rtdOutsideSealedEnclosure_, NUM_RTD_OUTSIDE_SEALED_ENCLOSURE);
  GETTER_SETTER_ARRAY(uint16_t, RtdVacuumJacket, rtdVacuumJacket_, NUM_RTD_VACUUM_JACKET);

  inline void setPressureTransducer(uint16_t v) { pressureTransducer_ = v; }
  inline uint16_t PressureTransducer() const { return pressureTransducer_; }

  GETTER_SETTER_ARRAY(uint16_t, Inclinometers, inclinometers_, NUM_INCLINOMETERS);
  GETTER_SETTER_ARRAY(uint16_t, RtdsInsideChamber, rtdsInsideChamber_, NUM_RTD_INSIDE_CHAMBER);
  GETTER_SETTER_ARRAY(uint16_t, Spare, spare_, NUM_SPARE);

  inline void setSealedEnclosurePressure(uint16_t v) { sealedEnclosurePressure_ = v; }
  inline uint16_t SealedEnclosurePressure() const { return sealedEnclosurePressure_; }

  inline void setSealedEnclosureTemperature(uint16_t v) { sealedEnclosureTemperature_ = v; }
  inline uint16_t SealedEnclosureTemperature() const { return sealedEnclosureTemperature_; }

  inline void setSealedEnclosureHumidity(uint16_t v) { sealedEnclosureHumidity_ = v; }
  inline uint16_t SealedEnclosureHumidity() const { return sealedEnclosureHumidity_; }

  GETTER_SETTER_ARRAY(uint16_t, PressureSensors, pressureSensors_, NUM_PRESSURE_SENSORS);
  GETTER_SETTER_ARRAY(uint16_t, Rtd4Wire, rtd4Wire_, NUM_4_WIRE_RTD);
  GETTER_SETTER_ARRAY(uint16_t, TofBiasVoltage, tofBiasVoltage_, NUM_TOF_BIAS);
  GETTER_SETTER_ARRAY(uint16_t, TofBiasTemperature, tofBiasTemperature_, NUM_TOF_BIAS);
  GETTER_SETTER_ARRAY(uint32_t, HubComputerErrorFlags, hubComputerErrorFlags_, NUM_ERROR_FLAGS);

  inline void setStorageSize(uint32_t v) { storageSize_ = v; }
  inline uint32_t StorageSize() const { return storageSize_; }

  inline void setCpuTemperature(uint16_t v) { cpuTemperature_ = v; }
  inline uint16_t CpuTemperature() const { return cpuTemperature_; }

  inline void setRamUsage(uint16_t v) { ramUsage_ = v; }
  inline uint16_t RamUsage() const { return ramUsage_; }
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_HubHKTelemetry_HH