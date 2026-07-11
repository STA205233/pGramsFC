#include "LabJackIO.hh"
#include <iostream>
namespace gramsballoon::pgrams {
using namespace LabJackAddresses;
LabJackIO::LabJackIO() {
  const LJM_ERROR_RETURN ret = LJM_CloseAll();
  if (ret != 0) {
    char err_str[LJM_STRING_MAX_SIZE];
    LJM_ErrorToString(ret, err_str);
    std::cerr << "Failed to close LabJack device: " << err_str << std::endl;
  }
  else {
    std::cout << "CloseAll of Labjack successful" << std::endl;
  }
}

int LabJackIO::open() {
  int handle;
  constexpr int num_trial = 10;
  for (int i = 0; i < num_trial; ++i) {
    LJM_ERROR_RETURN err = LJM_Open(LJM_dtANY, LJM_ctANY, "ANY", &handle);
    if (err == LJME_NOERROR) {
      break;
    }
    else {
      char err_str[LJM_STRING_MAX_SIZE];
      LJM_ErrorToString(err, err_str);
      std::cerr << "Failed to open LabJack device: " << err_str << std::endl;
    }
  }
  handle_ = handle;
  return 0;
}
int LabJackIO::close() {
  if (handle_ < 0) {
    return 0; // Not open, so nothing to do
  }
  LJM_ERROR_RETURN err = LJM_Close(handle_);
  if (err != LJME_NOERROR) {
    return err;
  }
  handle_ = -1;
  return 0;
}

int LabJackIO::configure(float32 range_setting, uint16_t all_negative_ch, uint16_t resolution_index, float32 all_setting_us) {
  if (handle_ < 0) {
    return -1; // Not open
  }
  int err = 0;
  err |= configOne(ALL_RANGE, range_setting);
  err |= configOne(ALL_NEGATIVE_CH, all_negative_ch);
  err |= configOne(ALL_RESOLUTION_INDEX, resolution_index);
  err |= configOne(ALL_SETTING_US, all_setting_us);
  return err;
}

int LabJackIO::acquireInfo() {
  if (handle_ < 0) {
    return -1; // Not open
  }

  const int err = LJM_GetHandleInfo(handle_, &deviceType_, &connectionType_, &serialNumber_, &ipAddress_, &port_, &maxBytesPerMB_);
  return err;
}
std::ostream &LabJackIO::printInfo(std::ostream &os) {
  if (deviceType_ == -1) {
    const auto err = acquireInfo();
    if (err != 0) {
      std::cerr << "Failed to acquire LabJack info" << std::endl;
      return os;
    }
  }
  char ip_str[LJM_IPv4_STRING_SIZE];
  LJM_NumberToIP(ipAddress_, ip_str);
  os << "-- LabJack Device Info --\n"
     << "Device Type: " << deviceType_ << "\n"
     << "Connection Type: " << connectionType_ << "\n"
     << "Serial Number: " << serialNumber_ << "\n"
     << "IP Address: " << ip_str << "\n"
     << "Port: " << port_ << "\n"
     << "Max Bytes Per MB: " << maxBytesPerMB_ << std::endl;
  return os;
}
} // namespace gramsballoon::pgrams
