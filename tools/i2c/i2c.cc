#include "BME680IO.hh"
#include "BayCatI2CIO.hh"
#include "I2CInterface.hh"
#include <iostream>
using namespace gramsballoon::pgrams;
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printUsage(std::cerr << "Error: invalid number of arguments." << std::endl);
    return 1;
  }
  BayCatI2CIO i2c;
  i2c.Open(0);
  const int test_mode = std::stoi(argv[1]);
  BME680IO bme680;
  switch (test_mode) {
  case 0:

    break;

  default:
    break;
  }
  return 0;
}
std::ostream &printUsage(std::ostream &stream) {
  stream << "Usage: i2c <test_mode>\n"
         << std::endl;
  return stream;
}
int test_1(BayCatI2CIO &i2c) {
  uint8_t reg_data;
  const auto ret = BME680IO::writeRegI2C(0xD0, &reg_data, 1, &i2c);
  std::cout << "writeRegI2C result: " << ret << std::endl;
  std::cout << "reg_data: " << static_cast<int>(reg_data) << std::endl;
  return ret;
}