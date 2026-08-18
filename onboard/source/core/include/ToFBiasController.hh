#ifndef GB_ToFBiasController_hh
#define GB_ToFBiasController_hh 1
#include "EncodedSerialCommunication.hh"
#include "HKDataSaver.hh"
#include "ToFBiasChannel.hh"
#include <cstdint>
#include <memory>
#include <ostream>
namespace gramsballoon::pgrams {

/**
 * @brief Control class for ToF Bias
 * @author Shota Arai
 * @date 2026-07-06 | Shota Arai | First version
 */
class ToFBiasController: public EncodedSerialCommunication, public HKDataSaver<uint8_t> {
  // for telemetry
  static constexpr int NUM_DATA = 536;
  // for command
  static constexpr int NUM_DCDC_SETTING = 8;
  static constexpr int NUM_TEMP_SETTING = 16;

public:
  ToFBiasController();
  ToFBiasController(const std::string& serial_path);
  virtual ~ToFBiasController();
  int enableDataStream();
  int disableDataStream();
  int queryFullOutput();
  int enableDCDC(uint8_t channel);
  int disableDCDC(uint8_t channel);
  std::ostream& printData(std::ostream& os);

  void setTimeOut(int sec, int usec) {
    timeOut_.tv_sec = sec;
    timeOut_.tv_usec = usec;
  }

  bool HasError() {
    return hasError_;
  }

  int initialize() override {
    const int ret = EncodedSerialCommunication::initialize();
    if (ret < 0) {
      hasError_ = true;
    }
    else {
      hasError_ = false;
    }
    return ret;
  }

  size_t Index() const { return index_; }

protected:
  using EncodedSerialCommunication::ReadData;
  using EncodedSerialCommunication::ReadDataUntilBreak;
  using EncodedSerialCommunication::ReadDataUntilSpecificStr;
  using EncodedSerialCommunication::setBaudrate;
  using EncodedSerialCommunication::setOpenMode;
  using EncodedSerialCommunication::sread;
  using EncodedSerialCommunication::sreadSingle;
  using EncodedSerialCommunication::swrite;
  using EncodedSerialCommunication::waitForReceivable;
  using EncodedSerialCommunication::WriteDataWithTimeout;

  int sendCommand(const std::string& data, timeval& timeout);
  int readData();
  void interpret(const char *dataStr);

private:
  char dataStr_[NUM_DATA];
  bool hasError_;
  size_t index_;
  std::unique_ptr<ToFBiasChannel<NUM_DCDC_SETTING>> channelForDCDC_ = nullptr;
  std::unique_ptr<ToFBiasChannel<NUM_TEMP_SETTING>> channelForTemp_ = nullptr;
  timeval timeOut_ = {0, 100000};
};

} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasController_hh