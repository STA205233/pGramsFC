#ifndef SerialCommunication_H
#define SerialCommunication_H 1

/**
 * A class of serial communication
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-02
 * @date 2026-08-22 | Shota Arai | Refactored
 */

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

class SerialCommunication {
public:
  SerialCommunication();
  SerialCommunication(const std::string& serial_path, speed_t baudrate, mode_t open_mode);
  virtual ~SerialCommunication();
  virtual int initialize();
  void setBaudrate(speed_t v) { baudrate_ = v; }
  void setSerialPath(const std::string& s) { serialPath_ = s; }
  void setOpenMode(mode_t c) { openMode_ = c; }
  void setTimeout(const std::chrono::microseconds& timeout) { timeout_ = timeout; }
  int Write(const uint8_t *data, int length);
  int Read(uint8_t *data, int length);
  int ReadExactly(uint8_t *data, int length);
  int FD() { return fd_; }
  bool IsOpen() const { return fd_ >= 0; }
  void Close();
  bool HasError() const { return hasError_; }
  int LastErrno() const { return lastErrno_; }
  void ClearError();

protected:
  int sread(uint8_t *buf, int length);
  int swrite(const uint8_t *buf, int length);
  int waitForReceivable(const std::chrono::microseconds &timeout);
  int waitForWritable(const std::chrono::microseconds &timeout);
  auto Timeout() const { return timeout_; }
  void setError(int err);
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

  static timeval calTimeVal(const std::chrono::microseconds &time);
  static bool isRecoverable(int err);
  template <typename FUNCTO, typename FUNCTX, typename T>
  int transferExactlyWithTimeout(FUNCTO functo, FUNCTX funcTX, T data, int length);
};

} // namespace gramsballoon::pgrams

#endif /* SerialCommunication_H */
