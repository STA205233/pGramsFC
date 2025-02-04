#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>

void SigPipeHandler(int) {
  std::cout << "Caught SIGPIPE!" << std::endl;
}

int main(int argc, char *argv[]) {
  const int num_trial = 10;
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <IP address> <port>" << std::endl;
    return 1;
  }
  bool failed = false;
  struct sigaction sa;
  sa.sa_handler = SigPipeHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGPIPE, &sa, NULL);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    std::cerr << "Error : Socket creation failed." << std::endl;
    return 1;
  }
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(std::atoi(argv[2]));
  serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
  for (int i = 0; i < num_trial; i++) {
    if (connect(sock, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
      std::cerr << "Error : Connection failed." << std::endl;
      failed = true;
      continue;
    }
    failed = false;
    break;
  }
  if (failed) {
    std::cerr << "Error : Connection failed, despite " << num_trial << " times trials." << std::endl;
    close(sock);
    return -1;
  }
  else {
    std::cout << "Connected to " << argv[1] << std::endl;
  }
  const std::string message = "Hello, world!";
  failed = false;
  for (int i = 0; i < num_trial; i++) {
    const auto send_result = send(sock, message.c_str(), message.size(), 0);
    if (send_result == -1) {
      std::cerr << "Error : Sending data failed." << std::endl;
      failed = true;
      continue;
    }
    failed = false;
    break;
  }
  close(sock);
  return 0;
}