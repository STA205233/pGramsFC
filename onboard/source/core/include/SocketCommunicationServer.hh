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
  size_t maximum_bytes_ = 1024 * 1024;
  int timeoutSec_ = 0;
  int timeoutUsec_ = 0;
  struct timeval timeoutTv_;
  template <typename T>
  int receiveImpl(std::vector<T> &data);

public:
  void setMaximumBytes(size_t bytes) {
    maximum_bytes_ = bytes;
  }

  size_t getMaximumBytes() const {
    return maximum_bytes_;
  }

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
      boost::system::error_code ec;
      socketAccepted_->close(ec);
      socketAccepted_.reset();
    }
    boost::system::error_code ec;
    socket_->close(ec);
    if (ec) {
      std::cerr << "Error in SocketCommunication: " << ec.message() << std::endl;
      failed_->store(true, std::memory_order_release);
      return -1;
    }
    stopped_->store(true, std::memory_order_release);
    return 0;
  }
  std::string getIP() const {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    if (!socketAccepted_) return "";
    try {
      return socketAccepted_->remote_endpoint().address().to_string();
    }
    catch (const boost::system::system_error &e) {
      std::cerr << "Error in SocketCommunication: " << e.what() << std::endl;
      return "";
    }
  }
  int getPort() const {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    if (!socketAccepted_) return -1;
    try {
      return socketAccepted_->remote_endpoint().port();
    }
    catch (const boost::system::system_error &e) {
      std::cerr << "Error in SocketCommunication: " << e.what() << std::endl;
      return -1;
    }
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
    if (data.empty()) {
      return 0;
    }
    return send(data.data(), sizeof(T) * data.size());
  }

  template <typename T>
  int receive(std::vector<T> &data) {
    int ret = 0;
    std::lock_guard<std::mutex> lock(*sockMutex_);
    ret = receiveImpl(data);
    return ret;
  }

  template <typename T>
  int receiveWithTimeout(std::vector<T> &data) {
    std::lock_guard<std::mutex> lock(*sockMutex_);
    if (socketAccepted_) {
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
      return receiveImpl(data);
    }
    return -1;
  }
};

template <typename T>
int SocketCommunication::receiveImpl(std::vector<T> &data) {
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
    const auto n = std::min(sz / sizeof(T), maximum_bytes_ / sizeof(T));
    data.resize(n);
    const auto ret = socketAccepted_->read_some(boost::asio::buffer(data.data(), n * sizeof(T)), errorcode);
    if (errorcode) {
      std::cerr << "Receive Error in SocketCommunication: " << errorcode.message() << std::endl;
      return -1;
    }
    data.resize(ret / sizeof(T));
    return ret;
  }
  return -1;
}

} // namespace gramsballoon::pgrams
#endif //GB_SocketCommunicationServer_hh