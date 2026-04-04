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

// Utility function
template <typename T>
struct type_mapping {
  static constexpr int ljm_type() {
    if constexpr (std::is_same_v<T, float32>) {
      return LJM_FLOAT32;
    }
    else if constexpr (std::is_same_v<T, std::uint32_t>) {
      return LJM_UINT32;
    }
    else if constexpr (std::is_same_v<T, std::int32_t>) {
      return LJM_INT32;
    }
    else if constexpr (std::is_same_v<T, std::uint16_t>) {
      return LJM_UINT16;
    }
    else {
      static_assert(false, "Unsupported type for write");
    }
  }
};

// Address name
template <Address_t Ch>
static constexpr Address_t AIN = LJMaddress<Ch, 0, float32, 250>::value;
static constexpr Address_t TEMPERATURE_AIR_K = LJMaddress<0, 60050, float32, 1>::value;
static constexpr Address_t TEMPERATURE_DEVICE_K = LJMaddress<0, 60052, float32, 1>::value;
} // namespace LabJackAddresses
} // namespace gramsballoon::pgrams
#endif //GB_LabJackAddresses_hh