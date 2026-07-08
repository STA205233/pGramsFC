#ifndef SerialCommunication_H
#define SerialCommunication_H 1

/**
 * A class of serial communication
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-02
 */

#include "termios.h"
#include "unistd.h"
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <string>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

#ifdef __APPLE__
#define TCSETS TIOCSETA
#endif

namespace gramsballoon::pgrams {

class SerialCommunication {
public:
  SerialCommunication();
  SerialCommunication(const std::string &serial_path, speed_t baudrate, mode_t open_mode);
  virtual ~SerialCommunication();
  virtual int initialize();
  int sreadSingle(uint8_t &buf);
  int sread(std::vector<uint8_t> &buf, int length);
  int swrite(const std::vector<uint8_t> &buf);
  int waitForReceivable(timeval &timeout);
  int waitForWritable(timeval &timeout);
  void setBaudrate(speed_t v) { baudrate_ = v; }
  void setSerialPath(const std::string &s) { serialPath_ = s; }
  void setOpenMode(mode_t c) { openMode_ = c; }

  int FD() { return fd_; }

protected:
  template <class FUNCTO, class FUNCTX, class... ARGS>
  int transferWithTimeout(FUNCTO funcTO, FUNCTX funcTX, timeval &timeout, ARGS &&...args);

private:
  std::unique_ptr<termios> tio_ = nullptr;
  int fd_ = 0;
  speed_t baudrate_ = B9600;
  std::string serialPath_;
  mode_t openMode_;
};

template <class FUNCTO, class FUNCTX, class... ARGS>
inline int SerialCommunication::transferWithTimeout(FUNCTO funcTO, FUNCTX funcTX, timeval &timeout, ARGS &&...args) {
  const int ret_timeout = funcTO(timeout);
  if (ret_timeout == 0) {
    return 0;
  }
  else if (ret_timeout < 0) {
    return ret_timeout;
  }
  const int ret_tx = funcTX(std::forward<ARGS>(args)...);
  return ret_tx;
}

} // namespace gramsballoon::pgrams

#endif /* SerialCommunication_H */
