#ifndef GB_RTDConversionConstant_h
#define GB_RTDConversionConstant_h 1
#include "ConvertedHubHKTelemetry.hh"
#include "SystemOfUnits.hh"
namespace gramsballoon::pgrams::conversion {
namespace rtd {
// Conversion for RTDs
constexpr ConvertedHubHKTelemetry::floating_t CMN_OFFSET = 0;
} // namespace rtd

namespace mhadc {
// Conversion for MHADC
constexpr int ADC_MAX = 2715;
constexpr ConvertedHubHKTelemetry::floating_t ADC_MAX_FLOAT = static_cast<ConvertedHubHKTelemetry::floating_t>(ADC_MAX);
constexpr ConvertedHubHKTelemetry::floating_t VREF = 3.3 * units::volt;
constexpr ConvertedHubHKTelemetry::floating_t CMN_OFFSET = 0 * units::volt;
} // namespace mhadc

namespace bme680 {
// Conversion for BME680
constexpr ConvertedHubHKTelemetry::floating_t COEFF_BME680_PRESS = 0.1 * units::hPa;
constexpr ConvertedHubHKTelemetry::floating_t COEFF_BME680_TEMP = 0.1 * units::degC;
constexpr ConvertedHubHKTelemetry::floating_t COEFF_BME680_HUMID = 0.1 * units::percent;
} // namespace bme680

namespace inclinometer {
constexpr ConvertedHubHKTelemetry::floating_t X_OFFSET = 0 * units::degree;
constexpr ConvertedHubHKTelemetry::floating_t Y_OFFSET = 0 * units::degree;
constexpr ConvertedHubHKTelemetry::floating_t COEFF_INCLINOMETER = 60.0 / 5.0 * units::degree / units::volt; // \pm 30 degree with 0--5 V
constexpr ConvertedHubHKTelemetry::floating_t OFFSET_INCLINOMETER = 2.5 * units::volt;
} // namespace inclinometer

namespace pdu {
constexpr ConvertedHubHKTelemetry::floating_t V_REF = 3.3 * units::volt;
constexpr ConvertedHubHKTelemetry::floating_t PRESICION = 4096.0;
constexpr ConvertedHubHKTelemetry::floating_t CMN_OFFSET = 0.0 * units::volt;

constexpr ConvertedHubHKTelemetry::floating_t COEFF_SIPM_VOL = 1. / 0.0503;
constexpr ConvertedHubHKTelemetry::floating_t COEFF_SIPM_CUR = 1. * units::ampere / units::volt;
constexpr ConvertedHubHKTelemetry::floating_t COEFF_TPC_HV_CUR = 0.08 / 0.3408 * units::milliampere / units::volt;
} // namespace pdu

} // namespace gramsballoon::pgrams::conversion
#endif //GB_RTDConversionConstant_h