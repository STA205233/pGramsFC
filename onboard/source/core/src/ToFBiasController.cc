#include "ToFBiasController.hh"
#include <charconv>
#define TOF_BIAS_DEBUG 0

namespace gramsballoon::pgrams {
ToFBiasController::ToFBiasController(const std::string& serial_path) : EncodedSerialCommunication(serial_path, B115200, O_RDWR), HKDataSaver<uint8_t>(100000, "tof_bias_data", NUM_DATA * sizeof(uint16_t)), hasError_(false), index_(0) {
}
ToFBiasController::ToFBiasController() : ToFBiasController("/dev/ttyUSB0") {
  disableDataStream();
}
ToFBiasController::~ToFBiasController() {
  disableDataStream();
}

int ToFBiasController::sendCommand(const std::string& data, timeval& timeout) {
  {
    const int ret = WriteDataWithTimeout(data, timeout);
    if (ret < 0) {
      hasError_ = true;
      std::cerr << "TOFBiasController::sendCommand: failed to write command code" << ret << std::endl;
      return ret;
    }
    hasError_ = false;
  }
  {
    const int ret = ReadDataWithTimeout(dataStr_, sizeof(dataStr_), timeout);
    if (ret < 0) {
      hasError_ = true;
      std::cerr << "TOFBiasController::sendCommand: failed to receive command code" << ret << std::endl;
      return ret;
    }
  }
  std::cerr << "TOFBiasController::sendCommand: some error happens" << std::endl;
  return -1;
}

int ToFBiasController::readData() {
  int index = 0;
  {
    while (index < NUM_DATA - 1) {
      const int ret = ReadDataWithTimeout(&dataStr_[index], NUM_DATA - index, timeOut_);
      if (ret < 0) {
#if TOF_BIAS_DEBUG > 0
        std::cerr << "error in communication with ToF Bias" << std::endl;
#endif
        hasError_ = true;
        return ret;
      }
      if (ret == 0) {
#if TOF_BIAS_DEBUG > 0
        std::cerr << "Timeout while reading ToF Bias data (" << index << "/" << NUM_DATA << " bytes received)" << std::endl;
#endif
        return 0;
      }
      hasError_ = false;
      index += ret;
    }
    if (static_cast<size_t>(index) != NUM_DATA) {
      std::cerr << "Warning: readData() read " << index + 1 << " bytes, expected " << NUM_DATA << " bytes." << std::endl;
      return -1;
    }
  }
  return index;
}

int ToFBiasController::queryFullOutput() {
  const int ret = sendCommand("p", timeOut_);
  if (ret < 0) {
    std::cerr << "Failed to write data" << std::endl;
    return ret;
  }
  return ret;
}

int ToFBiasController::enableDataStream() {
  const int ret = sendCommand("data on", timeOut_);
  if (ret < 0) {
    std::cerr << "Failed to write data" << std::endl;
    return ret;
  }
  return ret;
}
int ToFBiasController::disableDataStream() {
  const int ret = sendCommand("data off ", timeOut_);
  return ret;
}

int ToFBiasController::enableDCDC(uint8_t channel) {
  channelForDCDC_->setValue(channel);
  std::string ch = channelForDCDC_->serialize();
  if (ch.empty()) {
    return -1;
  }
  return sendCommand("denable " + ch + " on ", timeOut_);
}

int ToFBiasController::disableDCDC(uint8_t channel) {
  channelForDCDC_->setValue(channel);
  std::string ch = channelForDCDC_->serialize();
  if (ch.empty()) {
    return -1;
  }
  return sendCommand("denable " + channelForDCDC_->serialize() + " off ", timeOut_);
}

std::ostream& ToFBiasController::printData(std::ostream& os) {
  const int ret = sendCommand("p ", timeOut_);
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

} // namespace gramsballoon::pgrams