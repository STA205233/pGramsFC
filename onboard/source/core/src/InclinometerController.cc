#include "InclinometerController.hh"
namespace gramsballoon::pgrams {
InclinometerController::InclinometerController() : SerialCommunication("/dev/ttyS0", B38400, O_RDWR | O_NONBLOCK), timeout_({0, 100}) {}
InclinometerController::InclinometerController(const std::string &serial_path, speed_t baudrate) : SerialCommunication(serial_path, baudrate, O_RDWR | O_NONBLOCK), timeout_({0, 100}) {}
void InclinometerController::setFlags(tcflag_t &c_cflag) {
  c_cflag |= (CS8 | CLOCAL);
  c_cflag &= (~PARENB);
  c_cflag |= (CSTOPB);
  c_cflag |= CREAD;
}
int InclinometerController::writeRead(const uint8_t *buf, int length, uint8_t *read_buf, int read_length) {
  const int write_status = swrite(buf, length);
  if (write_status < 0) {
    std::cout << "write failed" << std::endl;
    return write_status;
  }
  const int timeout_status = WaitForTimeOut(timeout_);
  if (timeout_status < 0) {
    std::cout << "select failed" << std::endl;
    return timeout_status;
  }
  else if (timeout_status == 0) {
    std::cout << "timeout" << std::endl;
    return 0;
  }
  const int read_status = sread(read_buf, read_length);
  if (read_status < 0) {
    std::cout << "read failed" << std::endl;
    return read_status;
  }
  return 0;
}
int InclinometerController::getXY(int32_t &x, int32_t &y) {
  constexpr int cmd_size = 8;
  constexpr int response_size = 8;
  x = 0;
  y = 0;
  const uint8_t buf[cmd_size] = "get-x&y";
  uint8_t read_buf[response_size] = {0};
  const int status = writeRead(buf, cmd_size - 1, read_buf, response_size); // cmd_size - 1 to exclude null terminator
  if (status < 0) {
    std::cout << "writeRead failed" << std::endl;
    return status;
  }
  if (status == 0) {
    std::cout << "no response" << std::endl;
    return status;
  }
  x = (read_buf[0] << 24) | (read_buf[1] << 16) | (read_buf[2] << 8) | read_buf[3];
  y = (read_buf[4] << 24) | (read_buf[5] << 16) | (read_buf[6] << 8) | read_buf[7];
  return 0;
}
int InclinometerController::getTemp(int16_t &temp) {
  constexpr int cmd_size = 8;
  constexpr int response_size = 2;
  temp = 0;
  const uint8_t buf[cmd_size] = "gettemp";
  uint8_t read_buf[response_size] = {0};
  const int status = writeRead(buf, cmd_size - 1, read_buf, response_size); // cmd_size - 1 to exclude null terminator
  if (status < 0) {
    std::cout << "writeRead failed" << std::endl;
    return status;
  }
  if (status == 0) {
    std::cout << "no response" << std::endl;
    return status;
  }
  temp = (read_buf[0] << 8) | read_buf[1];
  return 0;
}
} // namespace gramsballoon::pgrams