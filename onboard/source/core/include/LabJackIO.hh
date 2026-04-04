#ifndef GB_LabJackIO_hh
#define GB_LabJackIO_hh 1
#include "LabJackAddresses.hh"
#include "LabJackM.h"
namespace gramsballoon::pgrams {
/**
 * @brief A IO class for a LabJack Device.
 * @author Shota Arai
 * @note This class is under assumption that only one LabJack device is connected to the computer for now.
 * @date 2026-04-04  | First version
 */
class LabJackIO {
  using Address_t = LabJackAddresses::Address_t;

public:
  LabJackIO() = default;
  virtual ~LabJackIO() = default;
  int open();
  int close();
  template <typename T>
  int read(Address_t address, T &value);
  template <typename T>
  int write(Address_t address, const T &value);

private:
  int handle_ = -1;
};

template <typename T>
int LabJackIO::read(Address_t address, T &value) {
  if (handle_ < 0) {
    return -1; // Not open
  }
  constexpr int type = LabJackAddresses::type_mapping<T>::ljm_type();

  double temp_value; // LJM_eReadAddress requires a double pointer, so we read into a temporary variable
  const auto err = LJM_eReadAddress(handle_, address, type, &temp_value);
  value = static_cast<T>(temp_value);
  return err;
}

template <typename T>
int LabJackIO::write(Address_t address, const T &value) {
  if (handle_ < 0) {
    return -1; // Not open
  }
  constexpr int type = LabJackAddresses::type_mapping<T>::ljm_type();
  const auto err = LJM_eWriteAddress(handle_, address, type, value);
  return err;
}

} // namespace gramsballoon::pgrams
#endif //GB_LabJackIO_hh