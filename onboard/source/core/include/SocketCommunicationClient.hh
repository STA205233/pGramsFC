#if 0
#ifndef GB_SocketCommunicationClient_hh
#define GB_SocketCommunicationClient_hh 1
#include "sys/socket.h"
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <optional>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>
namespace gramsballoon::pgrams {
class SocketCommunication {
public:
  SocketCommunication(const std::string &ip, int port) {
    serverAddress_.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_port = htons(port);
  }
  virtual ~SocketCommunication() = default;

protected:
  SocketCommunication(const SocketCommunication &r) = default;

private:
  int socket_ = -1;
  int socketServer_ = -1;
  sockaddr_in serverAddress_;
  int numTrial_ = 10;
  bool failed_ = false;
  bool binded_ = false;
  std::optional<timeval> timeout_ = std::nullopt;
  ssize_t sendImpl(const void *buf, size_t n) {
    for (int i = 0; i < numTrial_; i++) {
      const auto send_result = ::send(socket_, buf, n, 0); // TODO: this depends on telemetry definition.
      if (send_result == -1) {
        std::cerr << "Error in SocketCommunication::mod_analyze: " << "Trial " << i << " Sending data failed." << std::endl;
        failed_ = true;
        continue;
      }
      failed_ = false;
      return send_result;
    }
    if (failed_) {
      std::cerr << "Error in SocketCommunication: Sending data failed, despite " << numTrial_ << " times trials." << std::endl;
    }
    return -1;
  }

public:
  int connect();
  static void HandleSIGPIPE();
  template <typename T>
  ssize_t send(const T &data);
  template <typename T>
  ssize_t receive(T &data);
  int close() {
    return ::close(socket_);
  }
  void setNumTrials(int numTrials) { numTrial_ = numTrials; }
  bool isFailed() { return failed_; }
  bool isOpened() { return socket_ < 0; }
  in_addr_t getIP() { return serverAddress_.sin_addr.s_addr; }
  int getPort() { return serverAddress_.sin_port; }
  int getSocket() { return socket_; }
  int WaitForTimeOut(timeval timeout);
  void setTimeout(timeval timeout) { timeout_ = timeout; }
  void setTimeout(std::optional<timeval> timeout) { timeout_ = timeout; }
};
template <typename T>
ssize_t SocketCommunication::send(const T &data) {
  return sendImpl(&data, sizeof(T));
}
template <>
inline ssize_t SocketCommunication::send(const std::vector<uint8_t> &data) {
  return sendImpl(data.data(), data.size());
}
template <typename T>
ssize_t SocketCommunication::receive(T &data) {
  if (timeout_) {
    if (WaitForTimeOut(timeout_.value()) == 0) {
      return 0;
    }
  }
  const auto ret = ::recv(socket_, &data, sizeof(T), 0);
  if (ret < 0) {
    std::cerr << "Error in SocketCommunication::receive: receiving data failed. error code = " << errno << "(" << strerror(errno) << ")" << std::endl;
  }
  return ret;
}
template <>
inline ssize_t SocketCommunication::receive(std::vector<uint8_t> &data) {
  if (timeout_) {
    if (WaitForTimeOut(timeout_.value()) == 0) {
      return 0;
    }
  }
  const auto ret = ::recv(socket_, data.data(), data.size(), 0);
  if (ret < 0) {
    std::cerr << "Error in SocketCommunication::receive: receiving data failed. error code = " << errno << "(" << strerror(errno) << ")" << std::endl;
  }
  return ret;
}
} // namespace gramsballoon::pgrams
#endif //GB_SocketCommunicationClient_hh
#endif