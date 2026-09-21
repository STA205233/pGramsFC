#ifndef GRAMSBalloon_EncodedSerialCommunication_hh
#define GRAMSBalloon_EncodedSerialCommunication_hh 1
#include "SerialCommunication.hh"
#include <chrono>
#include <optional>
#include <string>
#include <string_view>

namespace gramsballoon {
namespace pgrams {
/**
 * @brief A class of serial communication which handles the data as a string instead of a byte array
 * @author Shota Arai
 * @date 2025-02-** | Shota Arai | First version
 */
class EncodedSerialCommunication: public SerialCommunication {
public:
  EncodedSerialCommunication() = default;
  virtual ~EncodedSerialCommunication() = default;
  /**
   * @brief Constructor which sets the parameters of the serial port
   * @param[in] serial_path Path of the serial port
   * @param[in] baudrate Baudrate of the serial port
   * @param[in] open_mode Mode used to open the serial port
   */
  EncodedSerialCommunication(const std::string &serial_path, speed_t baudrate, mode_t open_mode) : SerialCommunication(serial_path, baudrate, open_mode) {}
  /**
   * @brief Read data until the given string is received, the given length is reached or the timeout expires
   * @param[out] data Buffer to store the read data, which includes the terminating string
   * @param[in] end String which terminates the reading
   * @param[in] max_length Maximum number of the bytes to be read
   * @param[out] found Set true if the terminating string is received
   * @param[in] first_timeout Timeout applied only to the first byte. The timeout set by setTimeout() is used for the following bytes
   * @return Number of the read bytes, or negative value on failure
   */
  int ReadDataUntilSpecificStr(std::string &data, const std::string &end, int max_length, bool &found, std::optional<std::chrono::microseconds> first_timeout = std::nullopt);
  /**
   * @brief Read data until the given string is received, the given length is reached or the timeout expires
   * @param[out] data Buffer to store the read data, which includes the terminating string
   * @param[in] end String which terminates the reading
   * @param[in] max_length Maximum number of the bytes to be read
   * @param[in] first_timeout Timeout applied only to the first byte. The timeout set by setTimeout() is used for the following bytes
   * @return Number of the read bytes, or negative value on failure
   */
  int ReadDataUntilSpecificStr(std::string &data, const std::string &end, int max_length, std::optional<std::chrono::microseconds> first_timeout = std::nullopt) {
    bool f;
    return ReadDataUntilSpecificStr(data, end, max_length, f, first_timeout);
  }
  /**
   * @brief Read data until a line feed is received, the given length is reached or the timeout expires
   * @param[out] data Buffer to store the read data
   * @param[in] mex_length Maximum number of the bytes to be read
   * @return Number of the read bytes, or negative value on failure
   */
  int ReadDataUntilBreak(std::string &data, int mex_length);
  /**
   * @brief Read data from the serial port, which may return fewer bytes than requested
   * @param[out] data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @return Number of the read bytes, or negative value on failure
   */
  int Read(std::string &data, int length);
  /**
   * @brief Read data from the serial port until the requested number of the bytes is received or the timeout expires
   * @param[out] data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @return Number of the read bytes, or negative value on failure
   */
  int ReadExactly(std::string &data, int length);
  /**
   * @brief Write data to the serial port
   * @param[in] data Data to be written
   * @return Number of the written bytes, or negative value on failure
   */
  int Write(std::string_view data);

private:
  using SerialCommunication::Read;
  using SerialCommunication::ReadExactly;
  using SerialCommunication::Write;
  /**
   * @brief Helper function of Read() and ReadExactly(), which resizes the string to the number of the received bytes
   * @param[in] func Function which reads the data into a byte array
   * @param[out] data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @return Number of the read bytes, or negative value on failure
   */
  template <typename FUNC>
  int impl(FUNC func, std::string &data, int length);
};
} // namespace pgrams
} /* namespace gramsballoon */
#endif //GRAMSBalloon_EncodedSerialCommunication_hh