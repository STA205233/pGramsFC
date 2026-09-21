#ifndef ErrorManager_H
#define ErrorManager_H 1

#include "BaseTelemetryDefinition.hh"
#include "magic_enum.hpp"
#include <iostream>
#include <string>
#include <vector>


namespace gramsballoon::pgrams {
enum class Subsystem : uint16_t;
enum class ErrorType {
  MODULE_ACCESS_ERROR = 0,
  INITALIZATION_ERROR = 1,
  MHADC_COM_ERROR = 2,
  TPC_DAQC_COM_ERROR = 3,
  TPC_DAQT_COM_ERROR = 4,
  TPC_QMC_COM_ERROR = 5,
  TPC_QMT_COM_ERROR = 6,
  TOF_DAQC_COM_ERROR = 7,
  TOF_DAQT_COM_ERROR = 8,
  ORCC_COM_ERROR = 9,
  ORCT_COM_ERROR = 10,
  MQTT_COM_ERROR = 11,
  PDU_0_COM_ERROR = 12,
  PDU_1_COM_ERROR = 13,
  PDU_2_COM_ERROR = 14,
  PDU_3_COM_ERROR = 15,
  PDU_4_COM_ERROR = 16,
  PDU_5_COM_ERROR = 17,
  PDU_6_COM_ERROR = 18,
  PDU_7_COM_ERROR = 19,
  PDU_TPC_HV_COM_ERROR = 20,
  TOF_BIAS_COM_ERROR = 21,
  FORMAT_ERROR_MHADC = 22,
  FORMAT_ERROR_TPC_DAQT = 23,
  FORMAT_ERROR_TPC_DAQC = 24,
  FORMAT_ERROR_TPC_QMT = 25,
  FORMAT_ERROR_TPC_QMC = 26,
  FORMAT_ERROR_TOF_DAQT = 27,
  FORMAT_ERROR_TOF_DAQC = 28,
  FORMAT_ERROR_ORCT = 29,
  FORMAT_ERROR_ORCC = 30,
  INVALID_COMMAND = 31,

  RTD_GONDOLA_FRAME_1_ERROR = 32,
  RTD_GONDOLA_FRAME_2_ERROR = 33,
  RTD_GONDOLA_FRAME_3_ERROR = 34,
  RTD_GONDOLA_FRAME_4_ERROR = 35,
  RTD_DAQ_CRATE_1_ERROR = 36,
  RTD_DAQ_CRATE_2_ERROR = 37,
  RTD_DAQ_CRATE_BU_ERROR = 38,
  RTD_SHAPER_FARADAY_CAGE_1_ERROR = 39,
  RTD_SHAPER_FARADAY_CAGE_2_ERROR = 40,
  RTD_SHAPER_BOARD_1_ERROR = 41,
  RTD_SHAPER_BOARD_2_ERROR = 42,
  RTD_SHAPER_BOARD_3_ERROR = 43,
  RTD_HUB_COMPUTER_1_ERROR = 44,
  RTD_HUB_COMPUTER_2_ERROR = 45,
  RTD_TOF_FPGA_ERROR = 46,
  RTD_COMPUTER_BU_ERROR = 47,
  RTD_SEALED_ENCLOSURE_1_ERROR = 48,
  RTD_SEALED_ENCLOSURE_2_ERROR = 49,
  RTD_VACUUM_JACKET_1_ERROR = 50,
  RTD_VACUUM_JACKET_2_ERROR = 51,
  RTD_VACUUM_JACKET_3_ERROR = 52,
  PRESSURE_REGURATOR_ERROR = 53,
  PRESSURE_TRANSDUCER_1_ERROR = 54,
  PRESSURE_TRANSDUCER_2_ERROR = 55,
  LIQUID_LEVEL_METER_ERROR = 56,
  INCLINOMETER_ERROR = 57,
  RTD_INSIDE_CHAMBER_1_ERROR = 58,
  RTD_INSIDE_CHAMBER_2_ERROR = 59,
  RTD_INSIDE_CHAMBER_3_ERROR = 60,
  RTD_INSIDE_CHAMBER_4_ERROR = 61,
  RTD_INSIDE_CHAMBER_5_ERROR = 62,
  SPARE_ERROR = 63,
  SHUTDOWN_ERROR = 64,
  GET_CAPACITY_ERROR = 65,
  GET_TEMPERATURE_ERROR = 66,
  GET_MEM_ERROR = 67,
  TOO_LONG_TELEMETRY_ORC = 68,
  TOO_LONG_TELEMETRY_DAQ = 69,
  TOO_LONG_TELEMETRY_QM = 70,
  TOO_LONG_TELEMETRY_TOF = 71,
  SHUTDOWN_REJECTED = 72,
  REBOOT_REJECTED = 73,
  SOFTWARE_STOP_REJECTED = 74,

  SPI_OPEN_ERROR = 100,
  //SPI_OPEN_ERROR_BAYCAT = 101,
  TOF_DAQ_COMMAND_ERROR = 102,
  LABJACK_CONNECT_ERROR = 103,
  LABJACK_READ_ERROR = 104,
  DAC_SETTING_ERROR = 105,
  ADC_ACCESS_ERROR = 106,
  OTHER_ERRORS = 127,
};

/**
 * @brief A class to handle software error
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-04-27
 * @date 2025-10-13 | Changed for pGRAMS
 */
class ErrorManager {
public:
  static constexpr int NUM_ERROR_FLAGS = 4;
  ErrorManager();
  /**
   * @brief Clear all the error flags
   */
  void resetError();
  /**
   * @brief Raise the flag of the given error
   * @param[in] v Error type to be raised
   */
  void setError(ErrorType v);
  /**
   * @brief Convert the name of an error type into its bit position
   * @param[in] s Name of the error type
   * @return Bit position of the error type, or -1 if the name is unknown
   */
  static int strToBit(const std::string &s);
  /**
   * @brief Convert the bit position of an error type into its name
   * @param[in] v Bit position of the error type
   * @return Name of the error type, or an empty string if the position is unknown
   */
  static std::string bitToStr(int v);

  /**
   * @brief Return all the error flags
   * @return Error flags, where each bit corresponds to one error type
   */
  std::array<uint32_t, NUM_ERROR_FLAGS> ErrorCode() { return errorCode_; }
  /**
   * @brief Return one word of the error flags
   * @param[in] i Index of the word
   * @return Error flags of the word, or 0 if the index is out of range
   */
  uint32_t ErrorCode(int i) {
    if (i < 0 || i > NUM_ERROR_FLAGS - 1) {
      std::cerr << "ErrorManager::ErrorCode: Index out of range. Returning 0." << std::endl;
      return 0;
    }
    return errorCode_[i];
  }
  /**
   * @brief Set all the error flags
   * @param[in] v Error flags to be set
   */
  void SetErrorCode(std::array<uint32_t, NUM_ERROR_FLAGS> v) { errorCode_ = v; }
  /**
   * @brief Print the names of the error types currently raised
   */
  void PrintError();

private:
  std::array<uint32_t, NUM_ERROR_FLAGS> errorCode_ = {0};

public:
  /**
   * @brief Return the communication error type of the given subsystem
   * @param[in] subsystem Subsystem which failed to communicate
   * @param[in] is_command Set true for an error on the command line, false for an error on the telemetry line
   * @return Corresponding error type, or OTHER_ERRORS if the subsystem has no such error type
   */
  static ErrorType GetDaqComErrorType(Subsystem subsystem, bool is_command) {
    switch (subsystem) {
    case Subsystem::COL:
      return is_command ? ErrorType::TPC_DAQC_COM_ERROR : ErrorType::TPC_DAQT_COM_ERROR;
    case Subsystem::QM:
      return is_command ? ErrorType::TPC_QMC_COM_ERROR : ErrorType::TPC_QMT_COM_ERROR;
    case Subsystem::TOF:
      return is_command ? ErrorType::TOF_DAQC_COM_ERROR : ErrorType::TOF_DAQT_COM_ERROR;
    case Subsystem::ORC:
      return is_command ? ErrorType::ORCC_COM_ERROR : ErrorType::ORCT_COM_ERROR;
    default:
      return ErrorType::OTHER_ERRORS;
    }
  }
  /**
   * @brief Return the format error type of the given subsystem
   * @param[in] subsystem Subsystem which sent or received the invalid data
   * @param[in] is_command Set true for an error on the command line, false for an error on the telemetry line
   * @return Corresponding error type, or OTHER_ERRORS if the subsystem has no such error type
   */
  static ErrorType GetDaqFormatErrorType(Subsystem subsystem, bool is_command) {
    switch (subsystem) {
    case Subsystem::COL:
      return is_command ? ErrorType::FORMAT_ERROR_TPC_DAQC : ErrorType::FORMAT_ERROR_TPC_DAQT;
    case Subsystem::QM:
      return is_command ? ErrorType::FORMAT_ERROR_TPC_QMC : ErrorType::FORMAT_ERROR_TPC_QMT;
    case Subsystem::TOF:
      return is_command ? ErrorType::FORMAT_ERROR_TOF_DAQC : ErrorType::FORMAT_ERROR_TOF_DAQT;
    case Subsystem::ORC:
      return is_command ? ErrorType::FORMAT_ERROR_ORCC : ErrorType::FORMAT_ERROR_ORCT;
    default:
      return ErrorType::OTHER_ERRORS;
    }
  }
};

} // namespace gramsballoon::pgrams

#endif /* ErrorManager_H */
