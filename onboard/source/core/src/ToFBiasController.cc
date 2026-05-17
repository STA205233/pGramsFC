#include "ToFBiasController.hh"
namespace gramsballoon::pgrams {
ToFBiasController::ToFBiasController(const std::string &serial_path) : EncodedSerialCommunication(serial_path, B9600, O_RDWR | O_NONBLOCK), HKDataSaver<uint8_t>(100000, "tof_bias_data", NUM_DATA * sizeof(uint16_t)) {
}
ToFBiasController::ToFBiasController() : ToFBiasController("/dev/ttyACM0") {}
ToFBiasController::~ToFBiasController() {
  disableDataStream();
  EncodedSerialCommunication::~EncodedSerialCommunication();
}

int ToFBiasController::readData() {
  ReadData(dataStr_, NUM_DATA);
  // TODO: interpretation
}
int ToFBiasController::enableDataStream() {
  return WriteData("data 1");
}
int ToFBiasController::disableDataStream() {
  return WriteData("data 0");
}

} // namespace gramsballoon::pgrams