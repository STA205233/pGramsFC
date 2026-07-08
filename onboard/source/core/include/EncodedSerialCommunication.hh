#ifndef GRAMSBalloon_EncodedSerialCommunication_hh
#define GRAMSBalloon_EncodedSerialCommunication_hh 1
#include "SerialCommunication.hh"

namespace gramsballoon {
namespace pgrams {
class EncodedSerialCommunication: public SerialCommunication {
public:
  EncodedSerialCommunication() = default;
  virtual ~EncodedSerialCommunication() = default;
  EncodedSerialCommunication(const std::string &serial_path, speed_t baudrate, mode_t open_mode) : SerialCommunication(serial_path, baudrate, open_mode) {}
  int ReadData(std::string &data, int length);
  int ReadData(char *data, int length);
  int ReadDataUntilSpecificStr(std::string &data, const std::string &end);
  int ReadDataUntilBreak(std::string &data);
  int WriteData(const std::string &data);
  int WriteDataWithTimeout(const std::string &data, timeval &timeout) {
    return transferWithTimeout(
        [this](timeval &t) { return waitForWritable(t); },
        [this, &data]() { return WriteData(data); },
        timeout);
  }
  int ReadDataWithTimeout(char *data, int length, timeval &timeout) {
    return transferWithTimeout(
        [this](timeval &t) { return waitForReceivable(t); },
        [this, data, length]() { return ReadData(data, length); },
        timeout);
  }

private:
  std::vector<uint8_t> buf_;
};
} // namespace pgrams
} /* namespace gramsballoon */
#endif //GRAMSBalloon_EncodedSerialCommunicationForASCII_hh