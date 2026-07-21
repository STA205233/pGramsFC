#ifndef GB_LabJackIO_hh
#define GB_LabJackIO_hh 1
#include "LabJackAddresses.hh"
#include "LabJackM.h"
#include <ostream>
namespace gramsballoon::pgrams {
/**
 * @brief A IO class for a LabJack Device.
 * @author Shota Arai
 * @note This class is under assumption that only one LabJack device is connected to the computer for now.
 * @date 2026-04-04  | First version
 */
class LabJackIO {
  using Address_t = LabJackAddresses::Address_t;
  using float32 = LabJackAddresses::float32;

public:
  LabJackIO();
  virtual ~LabJackIO() = default;
  int open();
  int close();
  bool isOpen() const { return handle_ != -1; }
  int configure(float32 range_setting = 0.1, uint16_t all_negative_ch = 199, uint16_t resolution_index = 0, float32 all_setting_us = 0);
  template <typename T>
  int read(Address_t address, T &value);
  template <typename T>
  int write(Address_t address, const T &value);

  int acquireInfo();
  std::ostream &printInfo(std::ostream &os);

private:
  int handle_ = -1;
  int deviceType_ = -1;
  int connectionType_ = -1;
  int serialNumber_ = -1;
  int ipAddress_ = -1;
  int port_ = -1;
  int maxBytesPerMB_ = -1;

  template <typename T>
  int configOne(Address_t address, const T &write_value);
};

template <typename T>
int LabJackIO::read(Address_t address, T &value) {
  if (handle_ < 0) {
    return -1; // Not open
  }
  int type;
  LJM_AddressToType(address, &type);
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
  int type;
  LJM_AddressToType(address, &type);
  const auto err = LJM_eWriteAddress(handle_, address, type, value);
  return err;
}

template <typename T>
int LabJackIO::configOne(Address_t address, const T &write_value) {
  const int write_err = write(address, write_value);
  if (write_err != 0) {
    return write_err;
  }
  T read_value;
  const int read_err = read(address, read_value);
  if (read_err != 0) {
    return read_err;
  }
  if (read_value != write_value) {
    return -1; // Verification failed
  }
  return 0;
}

} // namespace gramsballoon::pgrams
#endif //GB_LabJackIO_hh