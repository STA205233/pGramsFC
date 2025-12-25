#include "CommandBuilder.hh"
#include "CommandSaver.hh"
#include "CommandSender.hh"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace gramsballoon::pgrams;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Usage: send_command <topic> <command name> <arg list>" << std::endl;
    return 0;
  }

  uint16_t code = std::stoi(argv[2]);
  std::vector<int32_t> arg_array;
  for (int i = 3; i < argc; i++) {
    arg_array.push_back(std::stoi(argv[i]));
  }
  
  const int result_init = mosqpp::lib_init();
  if (result_init != 0) {
    std::cerr << "Initialization failed" << std::endl;
    return -1;
  }

  std::vector<uint8_t> command;
  gramsballoon::CommandBuilder command_builder;
  try {
    command = command_builder.make_byte_array(static_cast<uint16_t>(code), arg_array);
  }
  catch (gramsballoon::CommandException &e) {
    std::cout << "Command exception caught: " << e.print() << std::endl;
    return 1;
  }

  std::string host;
  if (const char* host__ = std::getenv("PGRAMS_MOSQUITTO_HOST")) {
     host = host__;
  }
  else {
    std::cerr << "host is not set" << std::endl;
    return -1;
  }
  int port = 0;
  if (const char* port__ = std::getenv("PGRAMS_MOSQUITTO_PORT")) {
    port = std::stoi(port__);
  }
  else {
    std::cerr << "port is not set" << std::endl;
    return -1;
  }
  std::string username = "";
  if (const char *username__ = std::getenv("PGRAMS_MOSQUITTO_USER")) {
    username = username__;
  }
  std::string password = "";
  if (const char *password__ = std::getenv("PGRAMS_MOSQUITTO_PASSWD")) {
    password = password__;
  }
  
  std::cout << "Host: " << host << std::endl;
  std::cout << "Port: " << port << std::endl;
  std::cout << "Username: " << username << std::endl;
  std::cout << "Password: " << password << std::endl;
  CommandSender sender(host, port);
  if (username != "" && password != "") {
    const int auth_result = sender.authentication(username, password);
    if (auth_result != 0) {
      std::cout << strerror(auth_result) << std::endl;
      std::cout << "Authentication error -> exit" << std::endl;
      return auth_result;
    }
  }
  const int result = sender.open_mosquitto();
  if (result != 0) {
    std::cout << strerror(result) << std::endl;
    std::cout << "Mosquitto connection error -> exit" << std::endl;
    return result;
  }
  const int length_sent = sender.send(argv[1], command);
  //std::cout << "Length sent: " << length_sent << std::endl;

  //gramsballoon::write_command(command, name);

  sender.close_mosquitto();
  mosqpp::lib_cleanup(); 
  return 0;
}
