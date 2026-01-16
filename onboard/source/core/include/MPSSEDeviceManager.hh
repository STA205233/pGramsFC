#ifndef GRAMSBalloon_MPSSEDeviceManager_hh
#define GRAMSBalloon_MPSSEDeviceManager_hh 1
#include "MPSSEController.hh"
#include "ftd2xx.h"
#include <ostream>
namespace gramsballoon::pgrams::mpsse {
/**
 * @brief Class to manage MPSSE devices.
 * @author Shota Arai
 * @date 2026-01-14 | Initial version
 */
class MPSSEDeviceManager {
public:
  MPSSEDeviceManager() = default;
  virtual ~MPSSEDeviceManager() = default;
  int getDeviceNum();
  std::ostream &printDeviceInfo(std::ostream &os);
  int openDevice(int index, MPSSEController &controller);

private:
  MPSSEDeviceManager(const MPSSEDeviceManager &) = delete;
};
} // namespace gramsballoon::pgrams::mpsse
#endif // GRAMSBalloon_MPSSEDeviceManager_hh