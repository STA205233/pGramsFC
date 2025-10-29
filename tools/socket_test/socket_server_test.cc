#include "boost/asio.hpp"
#include "thread"
#include <chrono>
#include <iostream>

class server {

  boost::asio::io_context ioContext_;
  boost::asio::ip::tcp::socket socket_;
  std::shared_ptr<boost::asio::ip::tcp::socket> new_socket_ = nullptr;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::atomic<bool> is_running_ = true;

  uint8_t buf[1024];

public:
  server(int port) : ioContext_(), socket_(ioContext_), acceptor_(ioContext_, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4(), port)) {
  }
  void accept() {
    if (!is_running_.load()) {
      std::cerr << "Server is not running." << std::endl;
      return;
    }
    acceptor_.async_accept(
        [this](const boost::system::error_code &error, boost::asio::ip::tcp::socket new_socket) {
          if (!error) {
            std::cout << "Accepted connection from " << new_socket.remote_endpoint().address().to_string() << ":" << new_socket.remote_endpoint().port() << std::endl;
            new_socket_ = std::make_shared<boost::asio::ip::tcp::socket>(std::move(new_socket));
            readAndWrite();
          }
          else {
            std::cerr << "Error in accept: " << error.message() << std::endl;
          }
        });
  }
  void readAndWrite() {
    if (!is_running_.load()) {
      std::cerr << "Server is not running." << std::endl;
      return;
    }
    if (new_socket_ == nullptr) {
      std::cerr << "No active socket to read and write." << std::endl;
      return;
    }
    if (new_socket_->is_open() == false) {
      std::cerr << "Socket is not open." << std::endl;
      return;
    }
    if (new_socket_->available() > 0)
      receive();
    else {
      std::cerr << "No data available to read." << std::endl;
    }
    if (strlen(reinterpret_cast<char *>(buf)) > 0) {
      std::cout << "Sending data: " << buf << std::endl;
      send(buf, strlen((char *)buf));
    }
    else {
      std::cerr << "No data to send." << std::endl;
    }
    readAndWrite();
  }
  void receive() {
    if (!is_running_.load()) {
      std::cerr << "Server is not running." << std::endl;
      return;
    }
    if (new_socket_ == nullptr) {
      std::cerr << "No active socket to receive data." << std::endl;
      return;
    }
    new_socket_->async_receive(boost::asio::buffer(buf, 1024),
                               [this](const boost::system::error_code &error, std::size_t bytes_transferred) {
                                 if (!error) {
                                   std::cout << "Received data " << bytes_transferred << " bytes: " << buf << std::endl;
                                   send(buf, bytes_transferred);
                                 }
                                 else {
                                   std::cerr << "Error in receive: " << error.message() << std::endl;
                                   new_socket_->close();
                                   std::cout << "Socket closed." << std::endl;
                                   new_socket_.reset();
                                 }
                               });
  }
  void send(uint8_t *data, size_t size) {
    if (!is_running_.load()) {
      std::cerr << "Server is not running." << std::endl;
      return;
    }
    if (new_socket_ == nullptr) {
      std::cerr << "No active socket to send data." << std::endl;
      return;
    }
    if (new_socket_->is_open() == false) {
      std::cerr << "Socket is not open." << std::endl;
      return;
    }
    new_socket_->async_send(boost::asio::buffer(data, size),
                            [this](const boost::system::error_code &error, std::size_t bytes_transferred) {
                              if (!error) {
                                std::cout << "Sent data " << bytes_transferred << " bytes." << std::endl;
                              }
                              else {
                                std::cerr << "Error in send: " << error.message() << std::endl;
                                new_socket_->close();
                                std::cout << "Socket closed." << std::endl;
                                new_socket_.reset();
                              }
                            });
  }
  void run() {
    ioContext_.run();
  }
  void close() {
    socket_.close();
    std::cout << "Socket closed." << std::endl;
  }
};
void SigPipeHandler(int) {
  std::cout << "Caught SIGPIPE!" << std::endl;
}

int main(int argc, char *argv[]) {
  const int num_trial = 10;
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
    return 1;
  }
  bool failed = false;
  struct sigaction sa;
  sa.sa_handler = SigPipeHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGPIPE, &sa, NULL);

  uint8_t buf[1024];
  int port = atoi(argv[1]);
  server s(port);
  std::cout << "Waiting for connection..." << std::endl;
  s.accept();
  s.readAndWrite();
  s.run();
  for (int i = 0; i < num_trial; ++i) {
    std::cerr << "Sleeping... " << i + 1 << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  s.close();
  std::cout << "Server closed." << std::endl;
  //t.join();
  return 0;
}