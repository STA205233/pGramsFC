#include "SerialCommunication.hh"
#include "unistd.h"
#include <chrono>
#include <errno.h>
#include <iostream>
namespace gramsballoon::pgrams {

SerialCommunication::SerialCommunication()
    : baudrate_(B9600), openMode_(O_RDWR | O_NONBLOCK), timeout_(0) {
  serialPath_ = "/dev/null";
  tio_ = std::make_unique<termios>();
}

SerialCommunication::SerialCommunication(const std::string &serial_path, speed_t baudrate, mode_t open_mode) : timeout_(0) {
  tio_ = std::make_unique<termios>();
  serialPath_ = serial_path;
  baudrate_ = baudrate;
  openMode_ = open_mode;
}
void SerialCommunication::Close() {
  if (fd_ >= 0) {
    close(fd_);
    fd_ = -1;
  }
}
SerialCommunication::~SerialCommunication() {
  Close();
}

int SerialCommunication::initialize() {
  Close();
  cfsetospeed(tio_.get(), baudrate_);
  cfsetispeed(tio_.get(), baudrate_);
  std::cout << "BAUDRATE was set to " << baudrate_ << std::endl;

  cfmakeraw(tio_.get());
  std::cerr << "Set to raw mode" << std::endl;

  fd_ = open(serialPath_.c_str(), O_RDWR | O_NONBLOCK);
  if (fd_ < 0) {
    std::cout << "Open Error" << std::endl;
    return -1;
  }
  std::cout << "Open Serial port" << std::endl;
  tio_->c_cflag |= CREAD;
  tio_->c_cflag |= CS8;
  tio_->c_cflag |= CLOCAL;
  tio_->c_cflag &= (~PARENB);

  int status = tcsetattr(fd_, TCSANOW, tio_.get());
  if (status != 0) {
    std::cerr << "tcsetattr failed" << std::endl;
    return -1;
  }

  status = ioctl(fd_, TCSETS, tio_.get());
  if (status != 0) {
    std::cout << "ioctl failed" << std::endl;
    return -1;
  }
  status = fcntl(fd_, F_SETFL, openMode_);
  if (status != 0) {
    std::cout << "fcntl failed" << std::endl;
    return -1;
  }

  return 0;
}

int SerialCommunication::sread(uint8_t *buf, int length) {
  const int status = read(fd_, buf, length);
  return status;
}

int SerialCommunication::swrite(const uint8_t *buf, int length) {
  const int ret = write(fd_, buf, length);
  return ret;
}

int SerialCommunication::waitForReceivable(const std::chrono::microseconds &timeout) {
  if (fd_ < 0) {
    return -1;
  }
  fd_set fdset;
  FD_ZERO(&fdset);
  FD_SET(fd_, &fdset);
  auto to = calTimeVal(timeout);
  int rv = select(fd_ + 1, &fdset, NULL, NULL, &to);
  return rv;
}

int SerialCommunication::waitForWritable(const std::chrono::microseconds &timeout) {
  if (fd_ < 0) {
    return -1;
  }
  fd_set fdset;
  FD_ZERO(&fdset);
  FD_SET(fd_, &fdset);
  auto to = calTimeVal(timeout);
  int rv = select(fd_ + 1, NULL, &fdset, NULL, &to);
  return rv;
}

int SerialCommunication::Write(const uint8_t *data, int length) {
  return transferExactlyWithTimeout(
      [this](const std::chrono::microseconds &to) { return waitForWritable(to); },
      [this](const uint8_t *d, int l) { return swrite(d, l); },
      data, length);
}

int SerialCommunication::Read(uint8_t *data, int length) {
  const int ret_timeout = waitForReceivable(timeout_);
  const int err = errno;
  if (ret_timeout == 0) {
    return 0;
  }
  else if (ret_timeout < 0 && (err == EINTR)) { return 0; }
  else if (ret_timeout < 0) {
    return ret_timeout;
  }
  const int ret = sread(data, length);
  const int err2 = errno;
  if (ret < 0 && (err2 == EAGAIN || err2 == EINTR)) {
    return 0;
  }
  return ret;
}

int SerialCommunication::ReadExactly(uint8_t *data, int length) {
  return transferExactlyWithTimeout(
      [this](const std::chrono::microseconds &to) { return waitForReceivable(to); },
      [this](uint8_t *d, int l) { return sread(d, l); },
      data, length);
}

timeval SerialCommunication::calTimeVal(const std::chrono::microseconds &time) {
  if (time <= std::chrono::microseconds::zero()) {
    return timeval{0, 0};
  }
  timeval to;
  const auto sec = std::chrono::duration_cast<std::chrono::seconds>(time);
  to.tv_sec = sec.count();
  to.tv_usec = std::chrono::duration_cast<std::chrono::microseconds>(time - sec).count();
  return to;
}

template <typename FUNCTO, typename FUNCTX, typename T>
int SerialCommunication::transferExactlyWithTimeout(FUNCTO functo, FUNCTX funcTX, T data, int length) {
  using std::chrono::steady_clock;

  int index = 0;
  const auto deadline = steady_clock::now() + timeout_;

  while (index < length && steady_clock::now() < deadline) {
    const int ret_to = functo(std::chrono::duration_cast<std::chrono::microseconds>(deadline - steady_clock::now()));
    if (ret_to < 0 && (errno == EINTR)) { continue; }
    else if (ret_to < 0) {
      std::cerr << "SerialCommunication:: error in timeout" << std::endl;
      return ret_to;
    }
    else if (ret_to == 0) {
      break;
    }

    const int ret = funcTX(&data[index], length - index);
    const int err = errno;
    if (ret == 0) {
      return index;
    }
    else if (ret < 0 && (err == EINTR || err == EAGAIN)) { continue; }
    else if (ret < 0) {
      std::cerr << "SerialCommunication:: error in transfer" << std::endl;
      return ret;
    }

    index += ret;
  }
  if (index != length) {
    std::cerr << "Warning: transferred " << index << " bytes, expected " << length << " bytes." << std::endl;
  }
  return index;
}

} /* namespace gramsballoon::pgrams */
