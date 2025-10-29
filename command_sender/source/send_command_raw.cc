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

  std::vector<uint8_t> command;
  gramsballoon::CommandBuilder command_builder;
  try {
    command = command_builder.make_byte_array(static_cast<uint16_t>(code), arg_array);
  }
  catch (gramsballoon::CommandException &e) {
    std::cout << "Command exception caught: " << e.print() << std::endl;
    return 1;
  }

  std::string host = std::getenv("PGRAMS_MOSQUITTO_HOST");
  std::string port_str = std::getenv("PGRAMS_MOSQUITTO_PORT");
  std::string username = std::getenv("PGRAMS_MOSQUITTO_USER");
  std::string password = std::getenv("PGRAMS_MOSQUITTO_PASSWD");

  const int port = std::stoi(port_str);
  std::cout << "Host: " << host << std::endl;
  std::cout << "Port: " << port << std::endl;
  std::cout << "Username: " << username << std::endl;
  std::cout << "Password: " << password << std::endl;
  CommandSender sender(host, port);
  const int result = sender.open_mosquitto();
  if (result != 0) {
    std::cout << strerror(result) << std::endl;
    std::cout << "Mosquitto connection error -> exit" << std::endl;
    return result;
  }
  if (username != "" && password != "") {
    const int auth_result = sender.authentication(username, password);
    if (auth_result != 0) {
      std::cout << strerror(auth_result) << std::endl;
      std::cout << "Authentication error -> exit" << std::endl;
      return auth_result;
    }
  }
  const int length_sent = sender.send(argv[1], command);
  //std::cout << "Length sent: " << length_sent << std::endl;

  //gramsballoon::write_command(command, name);

  sender.close_mosquitto();

  return 0;
}
