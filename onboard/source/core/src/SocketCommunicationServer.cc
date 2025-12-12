#include "SocketCommunicationServer.hh"
#include <iostream>
namespace gramsballoon::pgrams {
void SigPipeHander(int) {
  std::cout << "Caught SIGPIPE!" << std::endl;
}
SocketCommunication::SocketCommunication(int port) {
  if (!ioContext_) {
    //ioContext_ = std::make_shared<boost::asio::io_context>();
    failed_->store(true, std::memory_order_release);
  }
  socket_ = std::make_shared<boost::asio::ip::tcp::socket>(*ioContext_);
  acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(
      *ioContext_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
  failed_ = std::make_shared<std::atomic<bool>>(false);
  stopped_ = std::make_shared<std::atomic<bool>>(true);
  sockMutex_ = std::make_shared<std::mutex>();
}
SocketCommunication::SocketCommunication(std::shared_ptr<boost::asio::io_context> ioContext, int port) : ioContext_(ioContext) {
  failed_ = std::make_shared<std::atomic<bool>>(false);
  if (!ioContext_) {
    failed_->store(true, std::memory_order_release);
  }
  socket_ = std::make_shared<boost::asio::ip::tcp::socket>(*ioContext_);
  acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(
      *ioContext_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
  stopped_ = std::make_shared<std::atomic<bool>>(false);
  sockMutex_ = std::make_shared<std::mutex>();
}
SocketCommunication::~SocketCommunication() {
  if (socket_) {
    stopped_->store(true, std::memory_order_release);
    socket_->close();
    socket_.reset();
  }
  if (acceptor_) {
    acceptor_->close();
    acceptor_.reset();
  }
}
void SocketCommunication::accept() {
  auto self = shared_from_this();
  acceptor_->async_accept([this, self](const boost::system::error_code &error, boost::asio::ip::tcp::socket socket) {
    if (!error) {
      try {
        std::cout << "Accepted connection from " << socket.remote_endpoint().address().to_string() << ":" << socket.remote_endpoint().port() << "(Server port: " << acceptor_->local_endpoint().port() << ")" << std::endl;
      }
      catch (const boost::system::system_error &e) {
        std::cerr << "Error in SocketCommunication: " << e.what() << std::endl;
      }
      std::lock_guard<std::mutex> lock(*sockMutex_);
      if (!socketAccepted_) {
        socketAccepted_ = std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));
      }
      else {
        try {
          std::cout << "Socket is already accepted. Closing the old socket" << std::endl;
        }
        catch (const boost::system::system_error &e) {
          std::cerr << "Error in SocketCommunication: " << e.what() << std::endl;
        }
        socketAccepted_->close();
        *socketAccepted_ = std::move(socket);
      }
    }
    else {
      failed_->store(true, std::memory_order_release);
      std::cerr << "Error in SocketCommunication: Accept failed. Error Code: " << error.message() << std::endl;
    }
    if (stopped_->load(std::memory_order_acquire)) {
      std::cerr << "SocketCommunication: Stopped." << std::endl;
      return;
    }
    accept();
  });
}
int SocketCommunication::send(const void* buf, size_t n){
  if (stopped_->load(std::memory_order_acquire)){
    return 0;
  }
  std::lock_guard<std::mutex> lock(*sockMutex_);
  if (socketAccepted_) {
    boost::system::error_code errorcode;
    {
      const auto ret = boost::asio::write(*socketAccepted_, boost::asio::buffer(buf, n), errorcode);
      if (errorcode) {
        std::cerr << "Error in SocketCommunication: " << errorcode.message() << std::endl;
        failed_->store(true, std::memory_order_release);
        return errorcode.value();
      }
      if (ret == 0) {
        std::cerr << "Error in SocketCommunication: No data sent." << std::endl;
        return -1;
      }
      return ret;
    }
  }
  return -1;
}
void SocketCommunication::HandleSIGPIPE() {
  struct sigaction sa;
  sa.sa_handler = SigPipeHander;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGPIPE, &sa, NULL);
}
} // namespace gramsballoon::pgrams