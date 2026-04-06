#include "SerialCommunication.hh"
#include <chrono>
#include <thread>

namespace gramsballoon::pgrams {

SerialCommunication::SerialCommunication()
    : baudrate_(B9600), openMode_(O_RDWR | O_NONBLOCK) {
  serialPath_ = "/dev/null";
  tio_ = std::make_unique<termios>();
}

SerialCommunication::SerialCommunication(const std::string &serial_path, speed_t baudrate, mode_t open_mode) {
  tio_ = std::make_unique<termios>();
  serialPath_ = serial_path;
  baudrate_ = baudrate;
  openMode_ = open_mode;
}

SerialCommunication::~SerialCommunication() {
  close(fd_);
}

int SerialCommunication::initialize() {
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
  setFlags(tio_->c_cflag);

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

int SerialCommunication::sreadSingle(uint8_t &buf) {
  const int length = 1;
  const int status = read(fd_, &buf, length);
  return status;
}

int SerialCommunication::sread(std::vector<uint8_t> &buf, int length) {
  const int status = read(fd_, &buf[0], length);
  return status;
}
int SerialCommunication::sread(uint8_t *buf, int length) {
  const int status = read(fd_, buf, length);
  return status;
}

int SerialCommunication::swrite(const uint8_t *buf, int length) {
  constexpr int sleep_ms = 1;

  const int status = write(fd_, buf, length);
  if (status < 0) {
    return status;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
  return status;
}

int SerialCommunication::swrite(const std::vector<uint8_t> &buf) {
  return swrite(buf.data(), buf.size());
}

int SerialCommunication::WaitForTimeOut(timeval timeout) {
  fd_set fdset;
  FD_ZERO(&fdset);
  FD_SET(fd_, &fdset);
  int rv = select(fd_ + 1, &fdset, NULL, NULL, &timeout);
  return rv;
}
void SerialCommunication::setFlags(tcflag_t &c_cflag) {
  c_cflag |= CREAD;
  c_cflag |= CS8;
  c_cflag |= CLOCAL;
  //c_cflag |= PARENB;
  c_cflag &= (~PARENB);
  //c_cflag |= CSTOPB;
}
} // namespace gramsballoon::pgrams
