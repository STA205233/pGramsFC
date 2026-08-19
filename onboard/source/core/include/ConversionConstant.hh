#ifndef GB_RTDConversionConstant_h
#define GB_RTDConversionConstant_h 1
#include "SystemOfUnits.hh"
namespace gramsballoon::pgrams::conversion {
namespace rtd {
// Conversion for RTDs
constexpr double CMN_OFFSET = 0;
} // namespace rtd

namespace mhadc {
// Conversion for MHADC
constexpr int ADC_MAX = 2715;
constexpr double ADC_MAX_FLOAT = static_cast<double>(ADC_MAX);
constexpr double VREF = 3.3 * units::volt;
constexpr double CMN_OFFSET = 0 * units::volt;
} // namespace mhadc

namespace bme680 {
// Conversion for BME680
constexpr double COEFF_BME680_PRESS = 0.1 * units::hPa / units::ADC;
constexpr double COEFF_BME680_TEMP = 0.1 * units::degC / units::ADC;
constexpr double COEFF_BME680_HUMID = 0.1 * units::percent / units::ADC;
} // namespace bme680

namespace inclinometer {
constexpr double X_OFFSET = 0 * units::degree;
constexpr double Y_OFFSET = 0 * units::degree;
constexpr double COEFF_INCLINOMETER = 60.0 / 5.0 * units::degree / units::volt; // \pm 30 degree with 0--5 V
constexpr double OFFSET_INCLINOMETER = 2.5 * units::volt;
} // namespace inclinometer

namespace pdu {
constexpr double V_REF = 3.3 * units::volt;
constexpr double PRESICION = 4096.0;
constexpr double CMN_OFFSET = 0.0 * units::volt;

constexpr double COEFF_SIPM_VOL = 1. / 0.0503;
constexpr double COEFF_SIPM_CUR = 1. * units::ampere / units::volt;
constexpr double COEFF_SIPM_PREAMP_TEMP = 1. / 8e3 * units::kelvin / units::volt;
constexpr double COEFF_TPC_HV_CUR = 0.08 / 0.3408 * units::milliampere / units::volt;
constexpr double COEFF_TPC_HV_VOL = 1. / 0.3408;
constexpr double COEFF_TPC_HV_TEMP = 1e6 / 8.06e3 * units::kelvin / units::volt;
constexpr double COEFF_MAIN_BAT_CUR = 1.0 * units::ampere / units::volt;
constexpr double COEFF_MAIN_BAT_VOL = 0.1;
constexpr double COEFF_MAIN_BAT_TEMP = 1e6 / 9e3 * units::kelvin / units::volt;
constexpr double COEFF_CHARGE_PREAMP_M5V_VOL = -0.5;
constexpr double COEFF_CHARGE_PREAMP_P5V_VOL = 0.5;
constexpr double COEFF_CHARGE_PREAMP_TEMP = 1e6 / 8e3 * units::kelvin / units::volt;
constexpr double COEFF_TOF_P12V_CUR = 2.0 * units::ampere / units::volt;
constexpr double COEFF_TOF_P12V_VOL = 1. / 0.2;
constexpr double COEFF_TOF_P12V_TEMP = 1e6 / 9e3 * units::kelvin / units::volt;
constexpr double COEFF_CAEN_NEVIS_P3V3_VOL = 1. / 2.0;
constexpr double COEFF_CAEN_NEVIS_P5V_VOL = 1. / 2.0;
constexpr double COEFF_CAEN_NEVIS_M5V_VOL = 1. / 2.0;
constexpr double COEFF_CAEN_NEVIS_P12V_VOL = 1. / 0.167;
constexpr double COEFF_SHAPER_P3V3_VOL = 1. / 0.5;
constexpr double COEFF_SHAPER_M3V3_VOL = 1. / 0.5;
constexpr double COEFF_SHAPER_TEMP = 1e6 / 8e3 * units::kelvin / units::volt;
constexpr double COEFF_CAEN_NEVIS_PM5V_TEMP = 1e6 / 8e3 * units::kelvin / units::volt;
constexpr double COEFF_PRESSURE_REGULATOR_VOL = 1.0 * units::Bar / units::volt;
constexpr double COEFF_TOF_BIAS_P5V_TEMP = 1e6 / 9.09e3 * units::kelvin / units::volt;
constexpr double COEFF_CAEN_NEVIS_P12V_TEMP = 1e6 / 10e3 * units::kelvin / units::volt;
constexpr double COEFF_CAEN_NEVIS_P3V3_TEMP = 1e6 / 9.09e3 * units::kelvin / units::volt;
constexpr double COEFF_COMMS_BOARD_TEMP = 1e6 / 8e3 * units::kelvin / units::volt;
} // namespace pdu

namespace labjack {
constexpr double COEFF_TEMP = 0.01 * units::degC / units::ADC;
constexpr double COEFF_PRESS = 145.0 * units::psi / (10.0 * units::volt);
constexpr double COEFF_VOL = 0.01 * units::volt / units::ADC;
} // namespace labjack

namespace biascontrol {
constexpr double R1 = 5 * units::Mohm;
constexpr double R2 = 49.9 * units::kohm;
constexpr double COEFF_VBIAS = 2.5 / 65536. * ((R1 + R2) / R2) * units::volt / units::ADC;

constexpr double Rtrim = 13. * units::Mohm;
constexpr double Rset = 1 * units::Mohm;
constexpr double COEFF_VTRIM = 2.5 / 4096. * Rtrim / Rset * units::volt / units::ADC;

constexpr double COEFF_TMON = 500.0 / 65536 * units::kelvin / units::ADC;
} // namespace biascontrol

} // namespace gramsballoon::pgrams::conversion
#endif //GB_RTDConversionConstant_h