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
constexpr double COEFF_TPC_HV_CUR = 0.08 / 0.3408 * units::milliampere / units::volt;
constexpr double COEFF_TPC_HV_VOL = 1. / 0.3408;
constexpr double COEFF_TPC_HV_TEMP = 1e6 / 8.06e3 * units::kelvin / units::volt;
constexpr double COEFF_MAIN_BAT_CUR = 1.0 * units::ampere / units::volt;
constexpr double COEFF_MAIN_BAT_VOL = 0.1;
constexpr double COEFF_MAIN_BAT_TEMP = 1e6 / 9e3 * units::kelvin / units::volt;
} // namespace pdu

namespace labjack {
constexpr double COEFF_TEMP = 0.01 * units::degC / units::ADC;
constexpr double COEFF_PRESS = 145.0 * units::psi / (10.0 * units::volt);
constexpr double COEFF_VOL = 0.01 * units::volt / units::ADC;
} // namespace labjack

} // namespace gramsballoon::pgrams::conversion
#endif //GB_RTDConversionConstant_h