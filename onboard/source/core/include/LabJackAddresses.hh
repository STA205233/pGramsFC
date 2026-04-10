#ifndef GB_LabJackAddresses_hh
#define GB_LabJackAddresses_hh 1
#include "LabJackM.h"
#include <cstdint>
#include <limits>
#include <type_traits>
namespace gramsballoon::pgrams {
namespace LabJackAddresses {
using Address_t = std::uint16_t;
using float32 = float;

// meta programming for address calculation
template <Address_t Ch, Address_t offset, typename T, Address_t max_value>
struct LJMaddress {
  static_assert(Ch < max_value, "Channel number exceeds maximum value");
  static constexpr Address_t value = Ch * sizeof(T) / 2 + offset;
};


// Address name
template <Address_t Ch>
static constexpr Address_t AIN = LJMaddress<Ch, 0, float32, 250>::value;
static constexpr Address_t TEMPERATURE_AIR_K = LJMaddress<0, 60050, float32, 1>::value;
static constexpr Address_t TEMPERATURE_DEVICE_K = LJMaddress<0, 60052, float32, 1>::value;
template <Address_t Ch>
static constexpr Address_t RANGE = LJMaddress<Ch, 40000, float32, 250>::value;
} // namespace LabJackAddresses
} // namespace gramsballoon::pgrams
#endif //GB_LabJackAddresses_hh