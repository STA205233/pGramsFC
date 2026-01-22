#ifndef GRAMSBalloon_HubHKTelemetry_HH
#define GRAMSBalloon_HubHKTelemetry_HH 1
#include "BaseTelemetryDefinition.hh"
#include "ErrorManager.hh"
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
  static constexpr size_t NUM_TOF_BIAS = 130;
  static constexpr size_t NUM_RTD_IN_CHAMBER = 7;
  static constexpr size_t NUM_PDU_HV_TEMP = 2;
  static constexpr size_t NUM_PDU_SIPM = 6;
  static constexpr size_t ARGC = 616 / 4;
  static constexpr size_t NUM_PDU_WARM_TPC_SHAPER = 6;
  static constexpr size_t NUM_ERROR_FLAGS = ErrorManager::NUM_ERROR_FLAGS;
  static constexpr size_t NUM_RTD_GONDOLA = 4;
  static constexpr size_t NUM_RTD_SHAPER_BOARD = 6;
  static constexpr size_t NUM_PDU_CPU = 7;
  static constexpr size_t NUM_PDU_TOF_TELEMETRY = 6;
  static constexpr size_t NUM_SPARE = 11;

public:
  void serialize(DBFieldSink *sink) const override;
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
  std::array<uint16_t, NUM_PDU_HV_TEMP> pduHVTemp_ = {0};
  uint16_t pduComsBoardTemp_ = 0;
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
  std::array<uint16_t, NUM_PDU_TOF_TELEMETRY> pduToFTelemetry_ = {0}; // TBD
  uint16_t pduCaenNevisP12VVol_ = 0;
  uint16_t pduCaenNevisP12VCur_ = 0;
  uint16_t pduCaenNevisM5VVol_ = 0;
  uint16_t pduCaenNevisM5VCur_ = 0;
  uint16_t pduCaenNevisP5VCur_ = 0;
  uint16_t pduCaenNevisP5VVol_ = 0;
  uint16_t pduCaenNevisP3V3Cur_ = 0;
  uint16_t pduCaenNevisP3V3Vol_ = 0;
  uint16_t pduWarmTPCShaperPVol_ = 0;
  uint16_t pduCaenNevisPM5VTemp_ = 0;
  uint16_t pduWarmTPCShaperTemp_ = 0;
  uint16_t pduWarmTPCShaperMVol_ = 0;
  std::array<uint16_t, NUM_PDU_WARM_TPC_SHAPER> pduWarmTPCShaperPCur_ = {0};
  std::array<uint16_t, NUM_PDU_WARM_TPC_SHAPER> pduWarmTPCShaperMCur_ = {0};
  std::array<uint16_t, NUM_PDU_CPU> pduCPUCur_ = {0}; // TBD
  uint16_t reserved1_ = 0;
  std::array<uint16_t, NUM_PDU_CPU> pduCPUVol_ = {0}; // TBD
  uint16_t reserved2_ = 0;
  uint16_t pduTofP5VVol_ = 0;
  uint16_t pduTofP5VCur_ = 0;
  uint16_t pduTofTemp_ = 0;
  uint16_t pduTofBiasP5V0Vol_ = 0;
  uint16_t pduTofBiasP5V0Cur_ = 0;
  uint16_t pduTofBiasTemp_ = 0;
  uint16_t pduMainDCDCTemp_ = 0;

  // MHADC
  std::array<uint16_t, NUM_RTD_GONDOLA> rtdGondolaFrame_ = {0};
  uint16_t rtdDaqCrate1_ = 0;
  uint16_t rtdDaqCrate2_ = 0;
  uint16_t rtdDaqCrateBackup_ = 0;
  uint16_t rtdShaperFaradayCage1_ = 0;
  uint16_t rtdShaperFaradayCage2_ = 0;
  std::array<uint16_t, NUM_RTD_SHAPER_BOARD> rtdShaperBoard_ = {0};
  uint16_t rtdHubComputerLocation1_ = 0;
  uint16_t rtdHubComputerLocation2_ = 0;
  uint16_t rtdTofFpgas_ = 0;
  uint16_t rtdTof2_ = 0;
  uint16_t rtdSealedEnclosure1WaterTank_ = 0;
  uint16_t rtdSealedEnclosureLocation2_ = 0;
  uint16_t rtdVacuumJacket1_ = 0;
  uint16_t rtdVacuumJacket2_ = 0;
  uint16_t rtdVacuumJacket3_ = 0;
  uint16_t pressureRegulator_ = 0;
  uint16_t pressureTransducer_ = 0;
  uint16_t liquidLevelMeter_ = 0;
  uint16_t inclinometer_ = 0;
  std::array<uint16_t, NUM_RTD_IN_CHAMBER> rtdsInsideChamber_ = {0};
  std::array<uint16_t, NUM_SPARE> spare_ = {0};

  //Tof bias
  std::array<uint16_t, NUM_TOF_BIAS> tofBiasVoltage_ = {0};
  //std::array<uint16_t, NUM_TOF_BIAS> tofBiasSetting_ = {0};

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

  inline void setPduVolSiPM(const std::array<uint16_t, NUM_PDU_SIPM> &v) { pduVolSiPM_ = v; }
  inline const std::array<uint16_t, NUM_PDU_SIPM> &PduVolSiPM() const { return pduVolSiPM_; }
  inline void setPduVolSiPM(size_t idx, uint16_t v) {
    if (idx >= NUM_PDU_SIPM) {
      std::cerr << "setPduVolSiPM: index out of range: " << idx << std::endl;
      return;
    }
    pduVolSiPM_[idx] = v;
  }
  inline uint16_t PduVolSiPM(size_t idx) const {
    if (idx >= NUM_PDU_SIPM) {
      std::cerr << "PduVolSiPM: index out of range: " << idx << std::endl;
      return 0;
    }
    return pduVolSiPM_[idx];
  }

  inline void setPduCurSiPM(const std::array<uint16_t, NUM_PDU_SIPM> &v) { pduCurSiPM_ = v; }
  inline const std::array<uint16_t, NUM_PDU_SIPM> &PduCurSiPM() const { return pduCurSiPM_; }
  inline void setPduCurSiPM(size_t idx, uint16_t v) {
    if (idx >= NUM_PDU_SIPM) {
      std::cerr << "setPduCurSiPM: index out of range: " << idx << std::endl;
      return;
    }
    pduCurSiPM_[idx] = v;
  }
  inline uint16_t PduCurSiPM(size_t idx) const {
    if (idx >= NUM_PDU_SIPM) {
      std::cerr << "PduCurSiPM: index out of range: " << idx << std::endl;
      return 0;
    }
    return pduCurSiPM_[idx];
  }

  inline void setPduCurTPCHV(uint16_t v) { pduCurTPCHV_ = v; }
  inline uint16_t PduCurTPCHV() const { return pduCurTPCHV_; }

  inline void setPduHVTemp(const std::array<uint16_t, NUM_PDU_HV_TEMP> &v) { pduHVTemp_ = v; }
  inline const std::array<uint16_t, NUM_PDU_HV_TEMP> &PduHVTemp() const { return pduHVTemp_; }
  inline void setPduHVTemp(size_t idx, uint16_t v) {
    if (idx >= NUM_PDU_HV_TEMP) {
      std::cerr << "setPduHVTemp: index out of range: " << idx << std::endl;
      return;
    }
    pduHVTemp_[idx] = v;
  }
  inline uint16_t PduHVTemp(size_t idx) const {
    if (idx >= NUM_PDU_HV_TEMP) {
      std::cerr << "PduHVTemp: index out of range: " << idx << std::endl;
      return 0;
    }
    return pduHVTemp_[idx];
  }

  inline void setPduComsBoardTemp(uint16_t v) { pduComsBoardTemp_ = v; }
  inline uint16_t PduComsBoardTemp() const { return pduComsBoardTemp_; }

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

  inline void setPduToFTelemetry(const std::array<uint16_t, 6> &v) { pduToFTelemetry_ = v; }
  inline const std::array<uint16_t, 6> &PduToFTelemetry() const { return pduToFTelemetry_; }
  inline void setPduToFTelemetry(size_t idx, uint16_t v) {
    if (idx >= 6) {
      std::cerr << "setPduToFTelemetry: index out of range: " << idx << std::endl;
      return;
    }
    pduToFTelemetry_[idx] = v;
  }
  inline uint16_t PduToFTelemetry(size_t idx) const {
    if (idx >= 6) {
      std::cerr << "PduToFTelemetry: index out of range: " << idx << std::endl;
      return 0;
    }
    return pduToFTelemetry_[idx];
  }

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

  inline void setPduWarmTPCShaperPVol(uint16_t v) { pduWarmTPCShaperPVol_ = v; }
  inline uint16_t PduWarmTPCShaperPVol() const { return pduWarmTPCShaperPVol_; }

  inline void setPduCaenNevisPM5VTemp(uint16_t v) { pduCaenNevisPM5VTemp_ = v; }
  inline uint16_t PduCaenNevisPM5VTemp() const { return pduCaenNevisPM5VTemp_; }

  inline void setPduWarmTPCShaperTemp(uint16_t v) { pduWarmTPCShaperTemp_ = v; }
  inline uint16_t PduWarmTPCShaperTemp() const { return pduWarmTPCShaperTemp_; }

  inline void setPduWarmTPCShaperMVol(uint16_t v) { pduWarmTPCShaperMVol_ = v; }
  inline uint16_t PduWarmTPCShaperMVol() const { return pduWarmTPCShaperMVol_; }

  inline void setPduWarmTPCShaperPCur(const std::array<uint16_t, NUM_PDU_WARM_TPC_SHAPER> &v) { pduWarmTPCShaperPCur_ = v; }
  inline const std::array<uint16_t, NUM_PDU_WARM_TPC_SHAPER> &PduWarmTPCShaperPCur() const { return pduWarmTPCShaperPCur_; }
  inline void setPduWarmTPCShaperPCur(size_t idx, uint16_t v) {
    if (idx >= NUM_PDU_WARM_TPC_SHAPER) {
      std::cerr << "setPduWarmTPCShaperPCur: index out of range: " << idx << std::endl;
      return;
    }
    pduWarmTPCShaperPCur_[idx] = v;
  }
  inline uint16_t PduWarmTPCShaperPCur(size_t idx) const {
    if (idx >= NUM_PDU_WARM_TPC_SHAPER) {
      std::cerr << "PduWarmTPCShaperPCur: index out of range: " << idx << std::endl;
      return 0;
    }
    return pduWarmTPCShaperPCur_[idx];
  }

  inline void setPduWarmTPCShaperMCur(const std::array<uint16_t, NUM_PDU_WARM_TPC_SHAPER> &v) { pduWarmTPCShaperMCur_ = v; }
  inline const std::array<uint16_t, NUM_PDU_WARM_TPC_SHAPER> &PduWarmTPCShaperMCur() const { return pduWarmTPCShaperMCur_; }
  inline void setPduWarmTPCShaperMCur(size_t idx, uint16_t v) {
    if (idx >= NUM_PDU_WARM_TPC_SHAPER) {
      std::cerr << "setPduWarmTPCShaperMCur: index out of range: " << idx << std::endl;
      return;
    }
    pduWarmTPCShaperMCur_[idx] = v;
  }
  inline uint16_t PduWarmTPCShaperMCur(size_t idx) const {
    if (idx >= NUM_PDU_WARM_TPC_SHAPER) {
      std::cerr << "PduWarmTPCShaperMCur: index out of range: " << idx << std::endl;
      return 0;
    }
    return pduWarmTPCShaperMCur_[idx];
  }

  inline void setPduCPUCur(const std::array<uint16_t, 7> &v) { pduCPUCur_ = v; }
  inline const std::array<uint16_t, 7> &PduCPUCur() const { return pduCPUCur_; }
  inline void setPduCPUCur(size_t idx, uint16_t v) {
    if (idx >= 7) {
      std::cerr << "setPduCPUCur: index out of range: " << idx << std::endl;
      return;
    }
    pduCPUCur_[idx] = v;
  }
  inline uint16_t PduCPUCur(size_t idx) const {
    if (idx >= 7) {
      std::cerr << "PduCPUCur: index out of range: " << idx << std::endl;
      return 0;
    }
    return pduCPUCur_[idx];
  }

  inline void setReserved1(uint16_t v) { reserved1_ = v; }
  inline uint16_t Reserved1() const { return reserved1_; }

  inline void setPduCPUVol(const std::array<uint16_t, 7> &v) { pduCPUVol_ = v; }
  inline const std::array<uint16_t, 7> &PduCPUVol() const { return pduCPUVol_; }
  inline void setPduCPUVol(size_t idx, uint16_t v) {
    if (idx >= 7) {
      std::cerr << "setPduCPUVol: index out of range: " << idx << std::endl;
      return;
    }
    pduCPUVol_[idx] = v;
  }
  inline uint16_t PduCPUVol(size_t idx) const {
    if (idx >= 7) {
      std::cerr << "PduCPUVol: index out of range: " << idx << std::endl;
      return 0;
    }
    return pduCPUVol_[idx];
  }

  inline void setReserved2(uint16_t v) { reserved2_ = v; }
  inline uint16_t Reserved2() const { return reserved2_; }

  inline void setPduTofP5VVol(uint16_t v) { pduTofP5VVol_ = v; }
  inline uint16_t PduTofP5VVol() const { return pduTofP5VVol_; }

  inline void setPduTofP5VCur(uint16_t v) { pduTofP5VCur_ = v; }
  inline uint16_t PduTofP5VCur() const { return pduTofP5VCur_; }

  inline void setPduTofTemp(uint16_t v) { pduTofTemp_ = v; }
  inline uint16_t PduTofTemp() const { return pduTofTemp_; }

  inline void setPduTofBiasP5V0Vol(uint16_t v) { pduTofBiasP5V0Vol_ = v; }
  inline uint16_t PduTofBiasP5V0Vol() const { return pduTofBiasP5V0Vol_; }

  inline void setPduTofBiasP5V0Cur(uint16_t v) { pduTofBiasP5V0Cur_ = v; }
  inline uint16_t PduTofBiasP5V0Cur() const { return pduTofBiasP5V0Cur_; }

  inline void setPduTofBiasTemp(uint16_t v) { pduTofBiasTemp_ = v; }
  inline uint16_t PduTofBiasTemp() const { return pduTofBiasTemp_; }

  inline void setPduMainDCDCTemp(uint16_t v) { pduMainDCDCTemp_ = v; }
  inline uint16_t PduMainDCDCTemp() const { return pduMainDCDCTemp_; }

  inline void setRtdGondolaFrame(int index, uint16_t v) { rtdGondolaFrame_[index] = v; }
  inline uint16_t RtdGondolaFrame(int index) const { return rtdGondolaFrame_[index]; }

  inline void setRtdDaqCrate1(uint16_t v) { rtdDaqCrate1_ = v; }
  inline uint16_t RtdDaqCrate1() const { return rtdDaqCrate1_; }

  inline void setRtdDaqCrate2(uint16_t v) { rtdDaqCrate2_ = v; }
  inline uint16_t RtdDaqCrate2() const { return rtdDaqCrate2_; }

  inline void setRtdDaqCrateBackup(uint16_t v) { rtdDaqCrateBackup_ = v; }
  inline uint16_t RtdDaqCrateBackup() const { return rtdDaqCrateBackup_; }

  inline void setRtdShaperFaradayCage1(uint16_t v) { rtdShaperFaradayCage1_ = v; }
  inline uint16_t RtdShaperFaradayCage1() const { return rtdShaperFaradayCage1_; }

  inline void setRtdShaperFaradayCage2(uint16_t v) { rtdShaperFaradayCage2_ = v; }
  inline uint16_t RtdShaperFaradayCage2() const { return rtdShaperFaradayCage2_; }

  inline void setRtdShaperBoard(int index, uint16_t v) { rtdShaperBoard_[index] = v; }
  inline uint16_t RtdShaperBoard(int index) const {
    return rtdShaperBoard_[index];
  }

  inline void setRtdHubComputerLocation1(uint16_t v) { rtdHubComputerLocation1_ = v; }
  inline uint16_t RtdHubComputerLocation1() const { return rtdHubComputerLocation1_; }

  inline void setRtdHubComputerLocation2(uint16_t v) { rtdHubComputerLocation2_ = v; }
  inline uint16_t RtdHubComputerLocation2() const { return rtdHubComputerLocation2_; }

  inline void setRtdTofFpgas(uint16_t v) { rtdTofFpgas_ = v; }
  inline uint16_t RtdTofFpgas() const { return rtdTofFpgas_; }

  inline void setRtdTof2(uint16_t v) { rtdTof2_ = v; }
  inline uint16_t RtdTof2() const { return rtdTof2_; }

  inline void setRtdSealedEnclosure1WaterTank(uint16_t v) { rtdSealedEnclosure1WaterTank_ = v; }
  inline uint16_t RtdSealedEnclosure1WaterTank() const { return rtdSealedEnclosure1WaterTank_; }

  inline void setRtdSealedEnclosureLocation2(uint16_t v) { rtdSealedEnclosureLocation2_ = v; }
  inline uint16_t RtdSealedEnclosureLocation2() const { return rtdSealedEnclosureLocation2_; }

  inline void setRtdVacuumJacket1(uint16_t v) { rtdVacuumJacket1_ = v; }
  inline uint16_t RtdVacuumJacket1() const { return rtdVacuumJacket1_; }

  inline void setRtdVacuumJacket2(uint16_t v) { rtdVacuumJacket2_ = v; }
  inline uint16_t RtdVacuumJacket2() const { return rtdVacuumJacket2_; }

  inline void setRtdVacuumJacket3(uint16_t v) { rtdVacuumJacket3_ = v; }
  inline uint16_t RtdVacuumJacket3() const { return rtdVacuumJacket3_; }

  inline void setPressureRegulator(uint16_t v) { pressureRegulator_ = v; }
  inline uint16_t PressureRegulator() const { return pressureRegulator_; }

  inline void setPressureTransducer(uint16_t v) { pressureTransducer_ = v; }
  inline uint16_t PressureTransducer() const { return pressureTransducer_; }

  inline void setLiquidLevelMeter(uint16_t v) { liquidLevelMeter_ = v; }
  inline uint16_t LiquidLevelMeter() const { return liquidLevelMeter_; }

  inline void setInclinometer(uint16_t v) { inclinometer_ = v; }
  inline uint16_t Inclinometer() const { return inclinometer_; }

  inline void setRtdsInsideChamber(const std::array<uint16_t, NUM_RTD_IN_CHAMBER> &v) { rtdsInsideChamber_ = v; }
  inline const std::array<uint16_t, NUM_RTD_IN_CHAMBER> &RtdsInsideChamber() const { return rtdsInsideChamber_; }
  inline void setRtdsInsideChamber(int idx, uint16_t v) {
    if (idx >= static_cast<int>(NUM_RTD_IN_CHAMBER)) {
      std::cerr << "setRtdsInsideChamber: index out of range: " << idx << std::endl;
      return;
    }
    rtdsInsideChamber_[idx] = v;
  }
  inline uint16_t RtdsInsideChamber(int idx) const {
    if (idx >= static_cast<int>(NUM_RTD_IN_CHAMBER)) {
      std::cerr << "RtdsInsideChamber: index out of range: " << idx << std::endl;
      return 0;
    }
    return rtdsInsideChamber_[idx];
  }

  inline void setSpare(const std::array<uint16_t, 11> &v) { spare_ = v; }
  inline const std::array<uint16_t, 11> &Spare() const { return spare_; }
  inline void setSpare(size_t idx, uint16_t v) {
    if (idx >= 11) {
      std::cerr << "setSpare: index out of range: " << idx << std::endl;
      return;
    }
    spare_[idx] = v;
  }
  inline uint16_t Spare(size_t idx) const {
    if (idx >= 11) {
      std::cerr << "Spare: index out of range: " << idx << std::endl;
      return 0;
    }
    return spare_[idx];
  }

  inline void setTofBiasVoltage(const std::array<uint16_t, NUM_TOF_BIAS> &v) { tofBiasVoltage_ = v; }
  inline const std::array<uint16_t, NUM_TOF_BIAS> &TofBiasVoltage() const { return tofBiasVoltage_; }
  inline void setTofBiasVoltage(size_t idx, uint16_t v) {
    if (idx >= NUM_TOF_BIAS) {
      std::cerr << "setTofBiasVoltage: index out of range: " << idx << std::endl;
      return;
    }
    tofBiasVoltage_[idx] = v;
  }
  inline uint16_t TofBiasVoltage(size_t idx) const {
    if (idx >= NUM_TOF_BIAS) {
      std::cerr << "TofBiasVoltage: index out of range: " << idx << std::endl;
      return 0;
    }
    return tofBiasVoltage_[idx];
  }

  //inline void setTofBiasSetting(const std::array<uint16_t, NUM_TOF_BIAS> &v) { tofBiasSetting_ = v; }
  //inline const std::array<uint16_t, NUM_TOF_BIAS> &TofBiasSetting() const { return tofBiasSetting_; }
  //inline void setTofBiasSetting(size_t idx, uint16_t v) {
  //  if (idx >= NUM_TOF_BIAS) {
  //    std::cerr << "setTofBiasSetting: index out of range: " << idx << std::endl;
  //    return;
  //  }
  //  tofBiasSetting_[idx] = v;
  //}
  //inline uint16_t TofBiasSetting(size_t idx) const {
  //  if (idx >= NUM_TOF_BIAS) {
  //    std::cerr << "TofBiasSetting: index out of range: " << idx << std::endl;
  //    return 0;
  //  }
  //  return tofBiasSetting_[idx];
  //}

  inline void setHubComputerErrorFlags(const std::array<uint32_t, NUM_ERROR_FLAGS> &v) { hubComputerErrorFlags_ = v; }
  inline const std::array<uint32_t, NUM_ERROR_FLAGS> &HubComputerErrorFlags() const { return hubComputerErrorFlags_; }
  inline void setHubComputerErrorFlags(size_t idx, uint32_t v) {
    if (idx >= NUM_ERROR_FLAGS) {
      std::cerr << "setHubComputerErrorFlags: index out of range: " << idx << std::endl;
      return;
    }
    hubComputerErrorFlags_[idx] = v;
  }
  inline uint32_t HubComputerErrorFlags(size_t idx) const {
    if (idx >= NUM_ERROR_FLAGS) {
      std::cerr << "HubComputerErrorFlags: index out of range: " << idx << std::endl;
      return 0;
    }
    return hubComputerErrorFlags_[idx];
  }

  inline void setStorageSize(uint32_t v) { storageSize_ = v; }
  inline uint32_t StorageSize() const { return storageSize_; }

  inline void setCpuTemperature(uint16_t v) { cpuTemperature_ = v; }
  inline uint16_t CpuTemperature() const { return cpuTemperature_; }
  inline void setRamUsage(uint16_t v) { ramUsage_ = v; }
  inline uint16_t RamUsage() const { return ramUsage_; }
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_HubHKTelemetry_HH