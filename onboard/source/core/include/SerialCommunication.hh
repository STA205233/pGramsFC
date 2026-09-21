#ifndef SerialCommunication_H
#define SerialCommunication_H 1

#include "termios.h"
#include <chrono>
#include <cstdint>
#include <fcntl.h>
#include <memory>
#include <string>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef __APPLE__
#define TCSETS TIOCSETA
#endif

namespace gramsballoon::pgrams {

/**
 * @brief A class of serial communication
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-02
 * @date 2026-08-22 | Shota Arai | Refactored
 * @ingroup device_core
 */
class SerialCommunication {
public:
  SerialCommunication();
  /**
   * @brief Constructor which sets the parameters of the serial port
   * @param[in] serial_path Path of the serial port
   * @param[in] baudrate Baudrate of the serial port
   * @param[in] open_mode Mode used to open the serial port
   */
  SerialCommunication(const std::string& serial_path, speed_t baudrate, mode_t open_mode);
  virtual ~SerialCommunication();
  /**
   * @brief Open the serial port and apply the current settings
   * @return 0 on success, negative value on failure
   */
  virtual int initialize();
  /**
   * @brief Set the baudrate of the serial port
   * @param[in] v Baudrate, e.g. B9600
   */
  void setBaudrate(speed_t v) { baudrate_ = v; }
  /**
   * @brief Set the path of the serial port
   * @param[in] s Path of the serial port
   */
  void setSerialPath(const std::string& s) { serialPath_ = s; }
  /**
   * @brief Set the mode used to open the serial port
   * @param[in] c Open mode
   */
  void setOpenMode(mode_t c) { openMode_ = c; }
  /**
   * @brief Set the timeout of one read or write access
   * @param[in] timeout Timeout
   */
  void setTimeout(const std::chrono::microseconds& timeout) { timeout_ = timeout; }
  /**
   * @brief Write data to the serial port
   * @param[in] data Data to be written
   * @param[in] length Number of the bytes to be written
   * @return Number of the written bytes, or negative value on failure
   */
  int Write(const uint8_t *data, int length);
  /**
   * @brief Read data from the serial port, which may return fewer bytes than requested
   * @param[out] data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @return Number of the read bytes, or negative value on failure
   */
  int Read(uint8_t *data, int length);
  /**
   * @brief Read data from the serial port until the requested number of the bytes is received or the timeout expires
   * @param[out] data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @return Number of the read bytes, or negative value on failure
   */
  int ReadExactly(uint8_t *data, int length);
  /**
   * @brief Return the file descriptor of the serial port
   * @return File descriptor, or negative value if the port is not open
   */
  int FD() { return fd_; }
  /**
   * @brief Return whether the serial port is open
   * @return True if the serial port is open
   */
  bool IsOpen() const { return fd_ >= 0; }
  /**
   * @brief Close the serial port
   */
  void Close();
  /**
   * @brief Return whether an error occurred in the last access
   * @return True if an error occurred
   */
  bool HasError() const { return hasError_; }
  /**
   * @brief Return the errno recorded at the last error
   * @return Value of errno
   */
  int LastErrno() const { return lastErrno_; }
  /**
   * @brief Clear the error status
   */
  void ClearError();

protected:
  /**
   * @brief Read data from the file descriptor once
   * @param[out] buf Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @return Number of the read bytes, or negative value on failure
   */
  int sread(uint8_t *buf, int length);
  /**
   * @brief Write data to the file descriptor once
   * @param[in] buf Data to be written
   * @param[in] length Number of the bytes to be written
   * @return Number of the written bytes, or negative value on failure
   */
  int swrite(const uint8_t *buf, int length);
  /**
   * @brief Wait until the serial port becomes readable
   * @param[in] timeout Time to wait
   * @return Positive value if the port is readable, 0 on timeout, negative value on failure
   */
  int waitForReceivable(const std::chrono::microseconds &timeout);
  /**
   * @brief Wait until the serial port becomes writable
   * @param[in] timeout Time to wait
   * @return Positive value if the port is writable, 0 on timeout, negative value on failure
   */
  int waitForWritable(const std::chrono::microseconds &timeout);
  /**
   * @brief Return the timeout of one read or write access
   * @return Timeout
   */
  auto Timeout() const { return timeout_; }
  /**
   * @brief Record an error
   * @param[in] err Value of errno to be recorded
   */
  void setError(int err);
  /**
   * @brief Discard the data which has been received but not read yet
   */
  void flush();

private:
  std::unique_ptr<termios> tio_ = nullptr;
  int fd_ = -1;
  speed_t baudrate_ = B9600;
  std::string serialPath_;
  mode_t openMode_;
  std::chrono::microseconds timeout_;
  bool hasError_ = false;
  int lastErrno_ = 0;

  /**
   * @brief Convert a duration into a timeval
   * @param[in] time Duration to be converted
   * @return Converted timeval
   */
  static timeval calTimeVal(const std::chrono::microseconds &time);
  /**
   * @brief Return whether the access can be retried after the given error
   * @param[in] err Value of errno
   * @return True if the access can be retried
   */
  static bool isRecoverable(int err);
  /**
   * @brief Helper function of ReadExactly() and Write(), which repeats the transfer until the requested number of the bytes is transferred or the timeout expires
   * @param[in] functo Function which waits until the port becomes ready
   * @param[in] funcTX Function which transfers the data once
   * @param[in,out] data Data to be transferred, or buffer to store the received data
   * @param[in] length Number of the bytes to be transferred
   * @return Number of the transferred bytes, or negative value on failure
   */
  template <typename FUNCTO, typename FUNCTX, typename T>
  int transferExactlyWithTimeout(FUNCTO functo, FUNCTX funcTX, T data, int length);
};

} // namespace gramsballoon::pgrams

#endif /* SerialCommunication_H */
