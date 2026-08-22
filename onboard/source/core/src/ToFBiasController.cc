#include "ToFBiasController.hh"
#define TOF_BIAS_DEBUG 0

namespace gramsballoon::pgrams {
ToFBiasController::ToFBiasController(const std::string& serial_path) : EncodedSerialCommunication(serial_path, B115200, O_RDWR), HKDataSaver<std::string>(100000, "tof_bias_data", NUM_DATA * sizeof(uint8_t)) {
  dataStr_.reserve(NUM_DATA);
}
ToFBiasController::ToFBiasController() : ToFBiasController("/dev/ttyUSB0") {
  dataStr_.reserve(NUM_DATA);
}
ToFBiasController::~ToFBiasController() {
  disableDataStream();
}

int ToFBiasController::getOnePacket() {
  {
    const int ret = enableDataStream();
    if (ret < 0) {
      return ret;
    }
  } // namespace gramsballoon::pgrams

  {
    const int ret = ReadDataUntilSpecificStr(dataStr_, "\r\n");
    if (ret < 0) {
      return ret;
    }
    saveData(&dataStr_);
  }
  {
    const int ret = disableDataStream();
    if (ret < 0) {
      return ret;
    }
  }
  return 0;
}

int ToFBiasController::sendCommand(std::string_view data) {
  {
    const int ret = Write(data);
    if (ret < 0) {
      std::cerr << "TOFBiasController::sendCommand: failed to write command code" << ret << std::endl;
      return ret;
    }
  }
  {
    const int ret = ReadDataUntilSpecificStr(dataStr_, "\r\n");
    if (ret < 0) {
      std::cerr << "TOFBiasController::sendCommand: failed to receive command code" << ret << std::endl;
      return ret;
    }
    if (dataStr_ == "OK!\r\n") {
      return 0;
    }
    else {
      return -13;
    }
  }
}

int ToFBiasController::queryFullOutput() {
  const int ret = sendCommand("p\r\n");
  if (ret < 0) {
    std::cerr << "Failed to write data" << std::endl;
    return ret;
  }
  const int ret2 = ReadDataUntilSpecificStr(dataStr_, "\r\n");
  saveData(&dataStr_);
  return ret2;
}

int ToFBiasController::enableDataStream() {
  const int ret = sendCommand("data on\r\n");
  if (ret < 0) {
    std::cerr << "Failed to write data" << std::endl;
    return ret;
  }
  return ret;
}
int ToFBiasController::disableDataStream() {
  const int ret = sendCommand("data off\r\n");
  return ret;
}

int ToFBiasController::enableDCDC(uint8_t channel) {
  return sendCommand("denable " + std::to_string(static_cast<int>(channel)) + " on\r\n");
}

int ToFBiasController::disableDCDC(uint8_t channel) {
  return sendCommand("denable " + std::to_string(static_cast<int>(channel)) + " off\r\n");
}

std::ostream& ToFBiasController::printData(std::ostream& os) {
  const int ret = queryFullOutput();
  if (ret < 0) {
    os << "Failed to send print command: " << ret;
    return os;
  }
  os << dataStr_;
  return os;
}

} // namespace gramsballoon::pgrams