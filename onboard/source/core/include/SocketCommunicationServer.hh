#ifndef GB_SocketCommunicationServer_hh
#define GB_SocketCommunicationServer_hh 1

#include "boost/asio.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/bind/bind.hpp"
#include <atomic>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>
namespace gramsballoon::pgrams {
class SocketSession;
/**
 * @brief SocketCommunicationServer class for managing socket communication.
 *
 * This class provides a basic implementation of a socket communication server.
 * It is designed to be inherited by other classes that require socket communication functionality.
 *
 * @author Shota Arai
 * @date 2025-05-09 | first draft
 */
class SocketCommunication: public std::enable_shared_from_this<SocketCommunication> {
public:
  SocketCommunication(int port);
  SocketCommunication(std::shared_ptr<boost::asio::io_context> ioContext, int port);
  virtual ~SocketCommunication();

protected:
  SocketCommunication(const SocketCommunication &r) = default;

private:
  std::shared_ptr<boost::asio::io_context> ioContext_ = nullptr;
  std::shared_ptr<boost::asio::ip::tcp::socket> socket_ = nullptr;
  std::shared_ptr<boost::asio::ip::tcp::socket> socketAccepted_;
  std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_ = nullptr;
  std::shared_ptr<std::atomic<bool>> failed_ = nullptr;
  std::shared_ptr<std::atomic<bool>> stopped_ = nullptr;
  std::shared_ptr<std::mutex> sockMutex_ = nullptr;
  std::optional<int> timeout_ = 200; // ms
  int timeoutSec_ = 0;
  int timeoutUsec_ = 0;
  struct timeval timeoutTv_;

public:
  void accept();
  void setTimeout(int timeout_ms) {
    timeout_ = timeout_ms;
    timeoutSec_ = timeout_ms / 1000;
    timeoutUsec_ = (timeout_ms % 1000) * 1000;
  }
  std::optional<int> getTimeout() const {
    return timeout_;
  }
  bool isConnected() const {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    return socketAccepted_ && socketAccepted_->is_open();
  }
  bool isFailed() const {
    return failed_->load(std::memory_order_acquire);
  }
  void resetFailed() {
    failed_->store(false, std::memory_order_release);
  }
  int close() {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    if (ioContext_) {
      ioContext_->stop();
    }
    if (socketAccepted_) {
      socketAccepted_->close();
      socketAccepted_.reset();
    }
    socket_->close();
    stopped_->store(true, std::memory_order_release);
    return 0;
  }
  std::string getIP() const {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    return socketAccepted_->remote_endpoint().address().to_string();
  }
  int getPort() const {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    return socketAccepted_->remote_endpoint().port();
  }
  bool isOpened() const {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    return socket_->is_open();
  }
  int getSocket() const {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    return socket_->native_handle();
  }
  static void HandleSIGPIPE();
  int send(const void *buf, size_t n);
  template <typename T>
  int send(const std::vector<T> &data) {
    return send(&data[0], sizeof(T) * data.size());
  }
  template <typename T>
  int receiveImpl(std::vector<T> &data) {
    if (socketAccepted_) {
      boost::system::error_code errorcode;
      size_t sz = socketAccepted_->available(errorcode);
      if (errorcode) {
        std::cerr << "Receive Error in SocketCommunication: " << errorcode.message() << std::endl;
        return -1;
      }
      if (sz == 0) {
        return 0;
      }
      const auto n = sz / sizeof(T);
      data.resize(n);
      const auto ret = boost::asio::read(*socketAccepted_, boost::asio::buffer(&data[0], n), errorcode);
      if (errorcode) {
        std::cerr << "Receive Error in SocketCommunication: " << errorcode.message() << std::endl;
        return -1;
      }
      return ret;
    }
    return -1;
  }
  template <typename T>
  int receive(std::vector<T> &data, bool lock = true) {
    int ret = 0;
    if (lock) {
      std::lock_guard<std::mutex> lock(*sockMutex_);
      ret = receiveImpl(data);
    }
    else {
      ret = receiveImpl(data);
    }
    return ret;
  }
  template <typename T>
  int receiveWithTimeout(std::vector<T> &data) {
    if (socketAccepted_) {
      std::lock_guard<std::mutex> lock(*sockMutex_);
      if (timeout_) {
        auto sock = socketAccepted_->native_handle();
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        timeoutTv_.tv_sec = timeoutSec_;
        timeoutTv_.tv_usec = timeoutUsec_;
        const int result = select(sock + 1, &readfds, NULL, NULL, &timeoutTv_);
        if (result < 0) {
          std::cerr << "Error in SocketCommunication: select failed." << std::endl;
          return -1;
        }
        else if (result == 0) {
          std::cerr << "Error in SocketCommunication: Timeout." << std::endl;
          return 0;
        }
      }
      return receive(data, false);
    }
    return -1;
  }
};

} // namespace gramsballoon::pgrams
#endif //GB_SocketCommunicationServer_hh