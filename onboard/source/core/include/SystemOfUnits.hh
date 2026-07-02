#ifndef GB_SystemOfUnits_hh
#define GB_SystemOfUnits_hh 1

namespace gramsballoon::pgrams::units {

// Electric potential (base unit: volt)
constexpr double volt = 1.;
constexpr double millivolt = 1.e-3 * volt;
constexpr double kilovolt = 1.e+3 * volt;
constexpr double V = volt;
constexpr double mV = millivolt;
constexpr double kV = kilovolt;

// Electric current (base unit: ampere)
constexpr double ampere = 1.;
constexpr double milliampere = 1.e-3 * ampere;
constexpr double microampere = 1.e-6 * ampere;
constexpr double A = ampere;
constexpr double mA = milliampere;
constexpr double uA = microampere;

// Temperature (base unit: kelvin)
constexpr double kelvin = 1.;
constexpr double degC = kelvin;

// Plane angle (base unit: radian)
constexpr double radian = 1.;
constexpr double pi = 3.14159265358979323846;
constexpr double degree = (pi / 180.) * radian;

// Time (base unit: second)
constexpr double second = 1.;
constexpr double millisecond = 1.e-3 * second;
constexpr double ms = millisecond;

// Dimensionless ratio
constexpr double percent = 1.e-2;

constexpr double GB = 1024.0 * 1024.0 * 1024.0;
constexpr double MB = 1024.0 * 1024.0;
constexpr double kB = 1024.0;
constexpr double Byte = 1.0;

// Pressure (base unit: Pa)
constexpr double Pa = 1.0;
constexpr double hPa = 100.0 * Pa;
constexpr double Torr = 133.322 * Pa;
constexpr double Bar = 100000.0 * Pa;
constexpr double psi = 6894.76 * Pa;
constexpr double MPa = 1000000.0 * Pa;
} // namespace gramsballoon::pgrams::units

#endif //GB_SystemOfUnits_hh
