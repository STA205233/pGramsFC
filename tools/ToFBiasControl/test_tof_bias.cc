#include "ToFBiasController.hh"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
using namespace gramsballoon::pgrams;
int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " mode" << std::endl;
    return -1;
  }
  
  const std::string path = "/dev/ttyUSB0";
  
  const int mode = std::atoi(argv[1]);
  
  ToFBiasController c(path);
  c.setTimeout(std::chrono::microseconds(1000000));
  c.initialize();
  std::string a;
  a.reserve(100000);
  switch (mode) {
  case 0:
    c.getOnePacket(a);
    break;
  case 1:
    c.queryFullOutput();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    c.getFullOutput();
    break;
  case 2:
  default:
    return -1;
  }
  return 0;
}