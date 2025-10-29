#include "SocketCommunicationServer.hh"
#include "SocketSessionForReader.hh"
#include "thread"
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: socket_server_test_async <port>" << std::endl;
    return 1;
  }
  const int port = std::stoi(argv[1]);
  std::shared_ptr<gramsballoon::pgrams::SocketCommunication> server = std::make_shared<gramsballoon::pgrams::SocketCommunication>(port, false);
  server->accept();
  std::thread server_thread([&server]() {
    server->run();
  });
  std::cout << "Server is running on port " << port << std::endl;
  std::cout << "Press Enter to stop the server..." << std::endl;
  std::cin.get();
  server->close();
  server_thread.join();
  std::cout << "Server stopped." << std::endl;
  std::cout << "Socket closed." << std::endl;
  return 0;
}