#include "ToFBiasController.hh"
#include <charconv>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>
#define TOF_BIAS_DEBUG 0

namespace gramsballoon::pgrams {
ToFBiasController::ToFBiasController(const std::string &serial_path) : EncodedSerialCommunication(serial_path, B115200, O_RDWR), HKDataSaver<std::string>(100000, "tof_bias_data", NUM_DATA * sizeof(uint8_t)), firstTimeout_(1000000) {
  dataStr_.reserve(NUM_DATA);
  fullOutputStr_.reserve(NUM_FULL_DATA);
  cmdStr_.reserve(NUM_CMD_DATA);
}
ToFBiasController::ToFBiasController() : ToFBiasController("/dev/ttyUSB0") {
  dataStr_.reserve(NUM_DATA);
  fullOutputStr_.reserve(NUM_FULL_DATA);
  cmdStr_.reserve(NUM_CMD_DATA);
}
ToFBiasController::~ToFBiasController() {
  disableDataStream();
}

int ToFBiasController::getOnePacket(std::string &str) {
  {
    const int ret = enableDataStream();
    if (ret < 0) {
      refresh();
      return ret;
    }
  }

  {
    const int ret = ReadDataUntilSpecificStr(str, "\r\n", NUM_DATA, firstTimeout_);
    if (ret < 0) {
      refresh();
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
  refresh();
  {
    const int ret = Write(data);
    if (ret < 0) {
      std::cerr << "TOFBiasController::sendCommand: failed to write command code" << ret << std::endl;
      return ret;
    }
  }
  {
    constexpr char suffix[] = "OK!\r\n";
    const int ret = ReadDataUntilSpecificStr(dataStr_, suffix, 50);
    if (ret < 0) {
      std::cerr << "TOFBiasController::sendCommand: failed to receive command code" << ret << std::endl;
      return ret;
    }
    if (dataStr_.rfind(suffix) == (dataStr_.length() + 1 - sizeof(suffix))) {
      return 0;
    }
    else {
      std::cerr << "TOFBiasController::sendCommand: Cannot find OK (dataStr_: " << dataStr_ << ")" << std::endl;
      return -13;
    }
  }
}

int ToFBiasController::queryFullOutput() {
  refresh();
  fullOutputStr_.clear();
  const int ret = Write("p\r\n");
  if (ret < 0) {
    std::cerr << "ToFBiasController::queryFullOutput(): Failed to write data" << std::endl;
    return ret;
  }
  isFullOutputQueried_ = true;
  return ret;
}
int ToFBiasController::getFullOutput() {
  if (!isFullOutputQueried_) return -50;
  bool is_ready = false;
  const int ret2 = ReadDataUntilSpecificStr(dataStr_, "OK!\r\n", NUM_DATA, is_ready);
  if (ret2 < 0) {
    return ret2;
  }
  fullOutputStr_.insert(fullOutputStr_.end(), dataStr_.begin(), dataStr_.begin() + ret2);
  if (is_ready) {
    isFullOutputQueried_ = false;
    saveData(&fullOutputStr_);
    return ret2;
  }
  return 0;
}

int ToFBiasController::enableDataStream() {
  const int ret = sendCommand("data on\r\n");
  if (ret < 0) {
    std::cerr << "ToFBiasController::enableDataStream(): Failed to write data" << std::endl;
    return ret;
  }
  return ret;
}
int ToFBiasController::disableDataStream() {
  const int ret = sendCommand("data off\r\n");
  if (ret < 0) {
    std::cerr << "ToFBiasController::disableDataStream(): Failed to write data" << std::endl;
    return ret;
  }
  return ret;
}

void ToFBiasController::appendInt(int value) {
  char buf[12];
  const auto res = std::to_chars(buf, buf + sizeof(buf), value);
  if (res.ec != std::errc()) {
    return;
  }
  cmdStr_.append(buf, res.ptr);
}

int ToFBiasController::enableDCDC(int channel) {
  cmdStr_.clear();
  cmdStr_ += "enable ";
  appendInt(channel);
  cmdStr_ += " on\r\n";
  return sendCommand(cmdStr_);
}

int ToFBiasController::disableDCDC(int channel) {
  cmdStr_.clear();
  cmdStr_ += "enable ";
  appendInt(channel);
  cmdStr_ += " off\r\n";
  return sendCommand(cmdStr_);
}

int ToFBiasController::setVoffset(int voltage) {
  cmdStr_.clear();
  cmdStr_ += "voffset ";
  appendInt(voltage);
  cmdStr_ += "\r\n";
  return sendCommand(cmdStr_);
}

int ToFBiasController::setTmuxChannel(int channel, int on_off) {
  if (on_off != 0 && on_off != 1) {
    return -1;
  }
  cmdStr_.clear();
  cmdStr_ += "tmux ";
  appendInt(channel);
  cmdStr_ += (on_off == 1 ? " on\r\n" : " off\r\n");
  return sendCommand(cmdStr_);
}

int ToFBiasController::setVdef(int channel, int voltage) {
  cmdStr_.clear();
  cmdStr_ += "vdef ";
  appendInt(channel);
  cmdStr_ += ' ';
  appendInt(voltage);
  cmdStr_ += "\r\n";
  return sendCommand(cmdStr_);
}

int ToFBiasController::setMode(TofBiasMode mode) {
  return sendCommand(mode == TofBiasMode::DEBUG ? "mode debug\r\n" : "mode auto\r\n");
}

int ToFBiasController::refresh() {
  dataStr_.clear();
  const int ret = Read(dataStr_, NUM_DATA);
  dataStr_.clear();
  return ret;
}
} // namespace gramsballoon::pgrams
