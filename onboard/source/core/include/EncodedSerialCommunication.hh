#ifndef GRAMSBalloon_EncodedSerialCommunication_hh
#define GRAMSBalloon_EncodedSerialCommunication_hh 1
#include "SerialCommunication.hh"
#include <chrono>
#include <optional>
#include <string>
#include <string_view>

namespace gramsballoon {
namespace pgrams {
class EncodedSerialCommunication: public SerialCommunication {
public:
  EncodedSerialCommunication() = default;
  virtual ~EncodedSerialCommunication() = default;
  EncodedSerialCommunication(const std::string &serial_path, speed_t baudrate, mode_t open_mode) : SerialCommunication(serial_path, baudrate, open_mode) {}
  int ReadDataUntilSpecificStr(std::string &data, const std::string &end, int max_length, bool &found, std::optional<std::chrono::microseconds> first_timeout = std::nullopt);
  int ReadDataUntilSpecificStr(std::string &data, const std::string &end, int max_length, std::optional<std::chrono::microseconds> first_timeout = std::nullopt) {
    bool f;
    return ReadDataUntilSpecificStr(data, end, max_length, f, first_timeout);
  }
  int ReadDataUntilBreak(std::string &data, int mex_length);
  int Read(std::string &data, int length);
  int ReadExactly(std::string &data, int length);
  int Write(std::string_view data);

private:
  using SerialCommunication::Read;
  using SerialCommunication::ReadExactly;
  using SerialCommunication::Write;
  template <typename FUNC>
  int impl(FUNC func, std::string &data, int length);
};
} // namespace pgrams
} /* namespace gramsballoon */
#endif //GRAMSBalloon_EncodedSerialCommunication_hh