#include "CommandBuilder.hh"
#include "CommandSaver.hh"
#include "CommandSender.hh"
#include <boost/optional.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace gramsballoon::pgrams;

std::string mapping_subsystem_name(const std::string &subsystem_name) {
  if (subsystem_name == "HUB" || subsystem_name == "PDU") {
    return "Hub";
  }
  else if (subsystem_name == "ORC") {
    return "Orchestrator";
  }
  else if (subsystem_name == "TOF") {
    return "TOF";
  }
  else if (subsystem_name == "COL") {
    return "TPC";
  }
  else {
    return "DUMMY_SUBSYSTEM";
  }
}

std::string get_topic(const std::string &subsystem_name, const std::string &inifile_name) {
  const std::string mapped_name = mapping_subsystem_name(subsystem_name);
  boost::property_tree::ptree pt;
  try {
    boost::property_tree::ini_parser::read_ini(inifile_name, pt);
  }
  catch (boost::property_tree::ini_parser_error &e) {
    std::cerr << "INI parser error: " << e.message() << " in file " << e.filename() << " at line " << e.line() << std::endl;
    return "";
  }
  const boost::optional<std::string> topic_opt = pt.get_optional<std::string>(mapped_name + ".comtopic");
  if (!topic_opt) {
    std::cerr << "No topic found for subsystem " << mapped_name << std::endl;
    return "";
  }
  std::string str = topic_opt.get();
  str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
  return str;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: send_command <command name> <arg list>" << std::endl;
    return 0;
  }

  const std::string name(argv[1]);
  std::vector<int32_t> arg_array;
  for (int i = 2; i < argc; i++) {
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
#ifdef SUBSYSTEM_NAME
  const std::string ini_filename = "../../settings/network.cfg";
  const std::string topic = get_topic(SUBSYSTEM_NAME, ini_filename);
  if (topic == "") {
    std::cerr << "No topic found -> exit" << std::endl;
    return -1;
  }
  const int length_sent = sender.send(topic, command);
  std::cout << "Length sent: " << length_sent << std::endl;
  std::cout << "Command sent to topic: " << topic << std::endl;
  std::cout << "Subsystem: " << SUBSYSTEM_NAME << std::endl;
  std::cout << "Command name: " << name << std::endl;
  std::cout << "Arguments: ";
  for (const int32_t arg: arg_array) {
    std::cout << arg << " ";
  }
  std::cout << std::endl;
  gramsballoon::write_command(command, name);
#endif

  sender.close_mosquitto();

  return 0;
}
