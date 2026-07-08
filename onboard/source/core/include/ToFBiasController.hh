#ifndef GB_ToFBiasController_hh
#define GB_ToFBiasController_hh 1
#include "EncodedSerialCommunication.hh"
#include "HKDataSaver.hh"
#include "ToFBiasChannel.hh"
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <ostream>
#include <thread>
namespace gramsballoon::pgrams {

/**
 * @brief Control class for ToF Bias
 * @author Shota Arai
 * @date 2026-07-06 | Shota Arai | First version
 */
class ToFBiasController: public EncodedSerialCommunication, public HKDataSaver<uint8_t> {
  // for telemetry
  static constexpr int NUM_ADC = 128;
  static constexpr int NUM_TEMP = 32;
  static constexpr int NUM_DAC = 128;
  static constexpr int NUM_DATA = NUM_ADC * 4 + NUM_TEMP * 4 + NUM_DAC * 3;
  // for command
  static constexpr int NUM_DCDC_SETTING = 8;
  static constexpr int NUM_TEMP_SETTING = 16;
  static constexpr int NUM_TRIM_SETTING = 128;

public:
  ToFBiasController();
  ToFBiasController(const std::string &serial_path);
  virtual ~ToFBiasController();
  int enableDataStream();
  int disableDataStream();
  int setTrimVoltage(uint32_t channel0, uint32_t channel1, uint32_t channel2, uint32_t channel3, int voltage);
  int setDefaultTemperature(uint16_t channel, int temperature);
  int enableTemperatureControl();
  int disableTemperatureControl();
  int enableDCDC(uint8_t channel);
  int disableDCDC(uint8_t channel);
  std::ostream &printData(std::ostream &os);

  std::array<uint16_t, NUM_ADC> AdcData() {
    std::unique_lock<std::mutex> lock(mutexForData_);
    return adcData_;
  }
  std::array<uint16_t, NUM_TEMP> TempData() {
    std::unique_lock<std::mutex> lock(mutexForData_);
    return tempData_;
  }
  std::array<uint16_t, NUM_DAC> DacData() {
    std::unique_lock<std::mutex> lock(mutexForData_);
    return dacData_;
  }

  void setTimeOut(int sec, int usec) {
    timeOut_.tv_sec = sec;
    timeOut_.tv_usec = usec;
  }

  bool HasError() {
    return hasError_.load();
  }

  int initialize() override {
    if (isRunning_.load() == 1) {
      isRunning_.store(2);
      cond_.notify_all();
      thread_->join();
      thread_.reset();
    }
    const int ret = SerialCommunication::initialize();
    if (ret < 0) {
      hasError_.store(true);
    }
    else {
      hasError_.store(false);
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

  int WriteDataWithTimeoutForToF(const std::string &data, timeval &timeout);
  int readDataWithInterpret();
  void interpret(const char *dataStr);

private:
  std::array<uint16_t, NUM_ADC> adcData_;
  char dataStr_[NUM_DATA];
  std::array<uint16_t, NUM_TEMP> tempData_;
  std::array<uint16_t, NUM_DAC> dacData_;
  std::condition_variable cond_;
  std::atomic_int isRunning_; // 0: off, 1: running, 2: no thread
  std::atomic_bool hasError_;
  std::unique_ptr<std::thread> thread_;
  std::mutex mutexForData_;
  std::mutex mutexForCond_;
  std::mutex mutexForComm_;
  size_t index_;
  std::unique_ptr<ToFBiasChannel<NUM_DCDC_SETTING>> channelForDCDC_ = nullptr;
  std::unique_ptr<ToFBiasChannel<NUM_TEMP_SETTING>> channelForTemp_ = nullptr;
  std::unique_ptr<ToFBiasChannel<NUM_TRIM_SETTING>> channelForTrim_ = nullptr;
  timeval timeOut_ = {0, 100000};
};

} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasController_hh