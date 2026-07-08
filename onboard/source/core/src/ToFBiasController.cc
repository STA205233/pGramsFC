#include "ToFBiasController.hh"
#include <charconv>
#include <thread>
#define TOF_BIAS_DEBUG 0

namespace gramsballoon::pgrams {
ToFBiasController::ToFBiasController(const std::string &serial_path) : EncodedSerialCommunication(serial_path, B115200, O_RDWR), HKDataSaver<uint8_t>(100000, "tof_bias_data", NUM_DATA * sizeof(uint16_t)), isRunning_(2), hasError_(false), index_(0) {
  channelForDCDC_ = std::make_unique<ToFBiasChannel<8>>();
  channelForTemp_ = std::make_unique<ToFBiasChannel<16>>();
  channelForTrim_ = std::make_unique<ToFBiasChannel<128>>();
}
ToFBiasController::ToFBiasController() : ToFBiasController("/dev/ttyACM0") {}
ToFBiasController::~ToFBiasController() {
  disableDataStream();
  if (thread_) {
    isRunning_.store(2);
    cond_.notify_all();
    thread_->join();
  }
}

int ToFBiasController::WriteDataWithTimeoutForToF(const std::string &data, timeval &timeout) {
  const int ret = WriteDataWithTimeout(data, timeout);
  if (ret < 0) {
    hasError_.store(true);
  }
  else {
    hasError_.store(false);
  }
  return ret;
}

int ToFBiasController::readDataWithInterpret() {
  int index = 0;
  {
    std::unique_lock<std::mutex> commLock(mutexForComm_);
    while (index < NUM_DATA - 1) {
      const int ret = ReadDataWithTimeout(&dataStr_[index], NUM_DATA - index, timeOut_);
      if (ret < 0) {
#if TOF_BIAS_DEBUG > 0
        std::cerr << "error in communication with ToF Bias" << std::endl;
#endif
        hasError_.store(true);
        return ret;
      }
      if (ret == 0) {
#if TOF_BIAS_DEBUG > 0
        std::cerr << "Timeout while reading ToF Bias data (" << index << "/" << NUM_DATA << " bytes received)" << std::endl;
#endif
        return 0;
      }
      hasError_.store(false);
      index += ret;
    }
    if (static_cast<size_t>(index) != NUM_DATA) {
      std::cerr << "Warning: readData() read " << index + 1 << " bytes, expected " << NUM_DATA << " bytes." << std::endl;
      return -1;
    }
  }
  interpret(dataStr_);
  return index;
}

int ToFBiasController::enableDataStream() {
  int ret;
  {
    std::unique_lock<std::mutex> commLock(mutexForComm_);
    ret = WriteDataWithTimeoutForToF("data on", timeOut_);
    if (ret < 0) {
      std::cerr << "Failed to write data" << std::endl;
      return ret;
    }
  }
  if (isRunning_.load() == 2) {
    isRunning_.store(1);
    thread_ = std::make_unique<std::thread>([&]() {
      while (true) {
        const int is_running = isRunning_.load();
        if (is_running == 0) {
#if TOF_BIAS_DEBUG >= 0
          std::cout << "Stopping thread..." << std::endl;
#endif
          std::unique_lock<std::mutex> lock(mutexForCond_);
          cond_.wait(lock, [&] { return isRunning_.load() != 0; });
        }
        else if (is_running == 2) {
#if TOF_BIAS_DEBUG >= 0
          std::cout << "killing thread..." << std::endl;
#endif
          break;
        }
#if TOF_BIAS_DEBUG > 0
        std::cout << "Running thread..." << std::endl;
#endif
        readDataWithInterpret();
      }
    });
  }
  else {
    isRunning_.store(1);
    cond_.notify_one();
  }
  return ret;
}
int ToFBiasController::disableDataStream() {
  std::unique_lock<std::mutex> commLock(mutexForComm_);
  const int ret = WriteDataWithTimeoutForToF("data off ", timeOut_);
  if (ret > 0) {
    isRunning_.store(0);
  }
  return ret;
}

int ToFBiasController::setTrimVoltage(uint32_t channel0, uint32_t channel1, uint32_t channel2, uint32_t channel3, int voltage) {
  channelForTrim_->setValue(channel0, channel1, channel2, channel3);
  std::string ch = channelForTrim_->serialize();
  if (ch.empty()) {
    return -1;
  }
  std::unique_lock<std::mutex> commLock(mutexForComm_);
  return WriteDataWithTimeoutForToF("trim " + ch + " " + std::to_string(voltage) + " ", timeOut_);
}

int ToFBiasController::setDefaultTemperature(uint16_t channel, int temperature) {
  channelForTemp_->setValue(channel);
  std::string ch = channelForTemp_->serialize();
  if (ch.empty()) {
    return -1;
  }
  std::unique_lock<std::mutex> commLock(mutexForComm_);
  return WriteDataWithTimeoutForToF("temp " + ch + " " + std::to_string(temperature) + " ", timeOut_);
}

int ToFBiasController::enableTemperatureControl() {
  std::unique_lock<std::mutex> commLock(mutexForComm_);
  return WriteDataWithTimeoutForToF("tenable on ", timeOut_);
}

int ToFBiasController::disableTemperatureControl() {
  std::unique_lock<std::mutex> commLock(mutexForComm_);
  return WriteDataWithTimeoutForToF("tenable off ", timeOut_);
}

int ToFBiasController::enableDCDC(uint8_t channel) {
  channelForDCDC_->setValue(channel);
  std::string ch = channelForDCDC_->serialize();
  if (ch.empty()) {
    return -1;
  }
  std::unique_lock<std::mutex> commLock(mutexForComm_);
  return WriteDataWithTimeoutForToF("denable " + ch + " on ", timeOut_);
}

int ToFBiasController::disableDCDC(uint8_t channel) {
  channelForDCDC_->setValue(channel);
  std::string ch = channelForDCDC_->serialize();
  if (ch.empty()) {
    return -1;
  }
  std::unique_lock<std::mutex> commLock(mutexForComm_);
  return WriteDataWithTimeoutForToF("denable " + channelForDCDC_->serialize() + " off ", timeOut_);
}

std::ostream &ToFBiasController::printData(std::ostream &os) {
  std::unique_lock<std::mutex> commLock(mutexForComm_);
  const int ret = WriteDataWithTimeoutForToF("p ", timeOut_);
  if (ret < 0) {
    os << "Failed to send print command: " << ret;
    return os;
  }
  std::string buf;
  const int ret2 = EncodedSerialCommunication::ReadDataUntilBreak(buf);
  if (ret2 < 0) {
    os << "Failed to read print data: " << ret2;
    return os;
  }
  os << buf;
  return os;
}

void ToFBiasController::interpret(const char *dataStr) {
  const char *p = dataStr;
  std::unique_lock<std::mutex> lock(mutexForData_);
  for (auto &v: adcData_) {
    std::from_chars(p, p + 4, v, 16);
    p += 4;
  }
  for (auto &v: tempData_) {
    std::from_chars(p, p + 4, v, 16);
    p += 4;
  }
  for (auto &v: dacData_) {
    std::from_chars(p, p + 3, v, 16);
    p += 3;
  }
  ++index_;
}
} // namespace gramsballoon::pgrams