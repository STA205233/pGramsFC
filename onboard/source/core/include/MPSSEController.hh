#ifndef GB_ControlMPSSE_hh
#define GB_ControlMPSSE_hh 1
#include "ftd2xx.h"
#include <cstdint>
#include <vector>
namespace gramsballoon::pgrams::mpsse {

/**
 * @brief Class to control one MPSSE device.
 * @author Shota Arai
 * @date 2026-01-14 | Initial version
 */
class MPSSEController {
public:
  MPSSEController() = default;
  virtual ~MPSSEController();
  int initialize();
  void setReadTimeout(ULONG timeout) {
    readTimeout_ = timeout;
  }
  void setWriteTimeout(ULONG timeout) {
    writeTimeout_ = timeout;
  }
  void setLatencyTimer(UCHAR timer) {
    latencyTimer_ = timer;
  }
  static unsigned int calDivider(unsigned int baudrate); 
  int applySettings();
  int testConnection();
  void setBaudrate(unsigned int baudrate);
  int setSPIMode(int mode, bool byteMode = true, bool msbFirst = true);
  int writeSPI(uint8_t *data, unsigned int size, int cs);
  void setHandle(FT_HANDLE handle) {
    handle_ = handle;
  }
  int readCurrentPinStatus(uint16_t &status);
  int writeGPIO(int pin, bool value);
  unsigned int getBaudrate();
  int write_readSPI(uint8_t *dataToSend, unsigned int size, uint8_t *dataToReceive, int cs);
  int close();

private:
  FT_HANDLE handle_ = nullptr;
  ULONG readTimeout_ = 1000; // in milliseconds
  ULONG writeTimeout_ = 1000; // in milliseconds
  UCHAR latencyTimer_ = 16; // in milliseconds
  int spiMode_ = 0;
  unsigned int baudrate_ = 9600;
  bool opened_ = false;

  std::vector<uint8_t> cmdBuffer_; //Just to avoid reallocations. It should be refreshed before each use in one function call.

  uint8_t spiWriteCommand_ = 0x00;
  uint8_t spiReadCommand_ = 0x00;
  uint8_t spiWriteReadCommand_ = 0x00;

  MPSSEController(const MPSSEController &) = delete;
  int writeMPSSE(std::vector<uint8_t> &data);
  int writeMPSSE(uint8_t *data, unsigned int size);
  int readMPSSE(std::vector<uint8_t> &data, unsigned int size);
  int readMPSSE(uint8_t *data, unsigned int size);
  int disableLoopback();
};
} // namespace gramsballoon::pgrams::mpsse
#endif //GB_ControlMPSSE_hh