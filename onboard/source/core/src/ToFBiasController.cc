#include "ToFBiasController.hh"
#include <string>
#include <string_view>
#define TOF_BIAS_DEBUG 0

namespace gramsballoon::pgrams {
ToFBiasController::ToFBiasController(const std::string &serial_path) : EncodedSerialCommunication(serial_path, B115200, O_RDWR), HKDataSaver<std::string>(100000, "tof_bias_data", NUM_DATA * sizeof(uint8_t)) {
  dataStr_.reserve(NUM_DATA);
}
ToFBiasController::ToFBiasController() : ToFBiasController("/dev/ttyUSB0") {
  dataStr_.reserve(NUM_DATA);
}
ToFBiasController::~ToFBiasController() {
  disableDataStream();
}

int ToFBiasController::getOnePacket(std::string &str) {
  {
    const int ret = enableDataStream();
    if (ret < 0) {
      return ret;
    }
  }

  {
    const int ret = ReadDataUntilSpecificStr(str, "\r\n");
    if (ret < 0) {
      return ret;
    }
    saveData(&str);
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

int ToFBiasController::enableDCDC(int channel) {
  return sendCommand("denable " + std::to_string(static_cast<int>(channel)) + " on\r\n");
}

int ToFBiasController::disableDCDC(int channel) {
  return sendCommand("denable " + std::to_string(static_cast<int>(channel)) + " off\r\n");
}

int ToFBiasController::setVoffset(int voltage) {
  return sendCommand("voffset " + std::to_string(voltage) + "\r\n");
}

int ToFBiasController::setTmuxChannel(int channel, int on_off) {
  if (on_off != 0 && on_off != 1) {
    return -1;
  }
  return sendCommand("tmux " + std::to_string(channel) + (on_off == 1 ? " on\r\n" : " off\r\n"));
}

int ToFBiasController::setVdef(int channel, int voltage) {
  return sendCommand("tdef " + std::to_string(channel) + " " + std::to_string(voltage) + "\r\n");
}

std::ostream &ToFBiasController::printData(std::ostream &os) {
  const int ret = queryFullOutput();
  if (ret < 0) {
    os << "Failed to send print command: " << ret;
    return os;
  }
  os << dataStr_;
  return os;
}

} // namespace gramsballoon::pgrams