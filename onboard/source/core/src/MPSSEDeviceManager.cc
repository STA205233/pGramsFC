#include "MPSSEDeviceManager.hh"
#include "MPSSEUtil.hh"
#include <iostream>
namespace gramsballoon::pgrams::mpsse {
int MPSSEDeviceManager::getDeviceNum() {
  DWORD numDevices = 0;
  HANDLE_ERROR(FT_CreateDeviceInfoList(&numDevices));
  return static_cast<int>(numDevices);
}
std::ostream &MPSSEDeviceManager::printDeviceInfo(std::ostream &os) {
  DWORD numDevices = 0;
  if (FT_CreateDeviceInfoList(&numDevices)) {
    os << "Failed to get number of MPSSE devices." << std::endl;
    return os;
  }
  os << "Number of MPSSE devices: " << numDevices << std::endl;
  for (DWORD i = 0; i < numDevices; ++i) {
    FT_DEVICE_LIST_INFO_NODE devInfo;
    if (FT_GetDeviceInfoDetail(i, &devInfo.Flags, &devInfo.Type, &devInfo.ID, &devInfo.LocId, devInfo.SerialNumber, devInfo.Description, &devInfo.ftHandle)) {
      os << "Failed to get info for device " << i << std::endl;
      continue;
    }
    os << "Device " << i << ": " << std::endl;
    os << "  Flags: " << devInfo.Flags << std::endl;
    os << "  Type: " << devInfo.Type << std::endl;
    os << "  ID: " << devInfo.ID << std::endl;
    os << "  LocId: " << devInfo.LocId << std::endl;
    os << "  SerialNumber: " << devInfo.SerialNumber << std::endl;
    os << "  Description: " << devInfo.Description << std::endl;
    os << "  ftHandle: " << devInfo.ftHandle << std::endl;
  }
  return os;
}
int MPSSEDeviceManager::openDevice(int index, MPSSEController &controller) {
  const int numDevices = getDeviceNum();
  if (index < 0 || index >= numDevices) {
    std::cerr << "Invalid device index: " << index << std::endl;
    return -1;
  }
  FT_HANDLE handle;
  if (FT_Open(index, &handle) != FT_OK) {
    std::cerr << "Failed to open MPSSE device at index " << index << std::endl;
    return -1;
  }
  DBG("Opened the device");
  controller.setHandle(handle);
  return 0;
}
} // namespace gramsballoon::pgrams::mpsse