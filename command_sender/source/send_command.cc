#include "CommandBuilder.hh"
#include "CommandSaver.hh"
#include "CommandSender.hh"
#include <cstdint>
#include <iostream>
#include <string>

using namespace gramsballoon::pgrams;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Usage: send_command <topic> <command name> <arg list>" << std::endl;
    return 0;
  }

  const std::string name(argv[2]);
  std::vector<int32_t> arg_array;
  for (int i = 3; i < argc; i++) {
    arg_array.push_back(std::stoi(argv[i]));
  }

  std::vector<uint8_t> command;
  gramsballoon::CommandBuilder command_builder;
  try {
    command = command_builder.make_byte_array(name, arg_array);
  }
  catch (gramsballoon::CommandException &e) {
    std::cout << "Command exception caught: " << e.print() << std::endl;
    return 1;
  }

  std::ifstream ifs("../source/network.cfg");
  if (!ifs) {
    std::cout << "Failed to open network.cfg" << std::endl;
    return 1;
  }
  std::string host;
  std::string port_str;
  getline(ifs, host, '\n');
  getline(ifs, port_str, '\n');
  ifs.close();
  const int port = std::stoi(port_str);
  std::cout << "Host: " << host << std::endl;
  std::cout << "Port: " << port << std::endl;
  CommandSender sender(host, port);
  if (!sender.open_mosquitto()) {
    std::cout << "Mosquitto connection error -> exit" << std::endl;
    return -1;
  }

  const int length_sent = sender.send(argv[1], command);
  std::cout << "Length sent: " << length_sent << std::endl;

  gramsballoon::write_command(command, name);

  sender.close_mosquitto();

  return 0;
}
