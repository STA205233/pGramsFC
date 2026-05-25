#include "ToFBiasController.hh"
#include <charconv>
namespace gramsballoon::pgrams {
ToFBiasController::ToFBiasController(const std::string &serial_path) : EncodedSerialCommunication(serial_path, B9600, O_RDWR | O_NONBLOCK), HKDataSaver<uint8_t>(100000, "tof_bias_data", NUM_DATA * sizeof(uint16_t)) {
}
ToFBiasController::ToFBiasController() : ToFBiasController("/dev/ttyACM0") {}
ToFBiasController::~ToFBiasController() {
  disableDataStream();
  EncodedSerialCommunication::~EncodedSerialCommunication();
}

int ToFBiasController::readData() {
  const int ret = ReadData(dataStr_, NUM_DATA);
  if (ret < 0) {
    return ret;
  }
  if (static_cast<size_t>(ret) != NUM_DATA) {
    std::cerr << "Warning: readData() read " << ret << " bytes, expected " << NUM_DATA << " bytes." << std::endl;
    return -1;
  }
  interpret(dataStr_);
  return ret;
}
int ToFBiasController::enableDataStream() {
  return WriteData("data on");
}
int ToFBiasController::disableDataStream() {
  return WriteData("data off");
}

int ToFBiasController::setTrimVoltage(int channel, int voltage) {
  return WriteData("trim " + std::to_string(channel) + " " + std::to_string(voltage));
}

int ToFBiasController::setDefaultTemperature(int channel, int temperature) {
  return WriteData("temp " + std::to_string(channel) + " " + std::to_string(temperature));
}

int ToFBiasController::enableTemperatureControl(int channel) {
  return WriteData("tenable " + std::to_string(channel) + " on");
}

int ToFBiasController::disableTemperatureControl(int channel) {
  return WriteData("tenable " + std::to_string(channel) + " off");
}

std::ostream &ToFBiasController::printData(std::ostream &os) {
  const int ret = WriteData("p");
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
  for (auto &v: adcData_) {
    std::from_chars(p, p + 4, v, 16);
    p += 4;
  }
  for (auto &v: tempData_) {
    std::from_chars(p, p + 2, v, 16);
    p += 4;
  }
  for (auto &v: dacData_) {
    std::from_chars(p, p + 3, v, 16);
    p += 3;
  }
}
} // namespace gramsballoon::pgrams