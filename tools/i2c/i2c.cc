#include "BME280IO.hh"
#include "BME680IO.hh"
#include "BayCatI2CIO.hh"
#include "I2CInterface.hh"
#include <iostream>
using namespace gramsballoon::pgrams;
std::ostream &printUsage(std::ostream &);
int test_1(BayCatI2CIO &);
int test_2(BayCatI2CIO &);
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printUsage(std::cerr << "Error: invalid number of arguments." << std::endl);
    return 1;
  }
  BayCatI2CIO i2c;
  i2c.Open(0);
  const int test_mode = std::stoi(argv[1]);
  //BME280IO bme280;
  switch (test_mode) {
  case 0:
    return test_1(i2c);
    break;
  case 1:
    return test_2(i2c);
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
  uint8_t reg_data[] = {0x00};
  //const auto ret2 = BME280IO::writeRegI2C(0xE0, /reg_data, 1, &i2c);
  const auto ret = BME680IO::readRegI2C(0xD0, reg_data, 1, &i2c);
  std::cout << "readRegI2C result: " << static_cast<int>(ret) << std::endl;
  std::cout << std::hex << "reg_data: 0x" << static_cast<int>(reg_data[0]) << std::dec << std::endl;
  return ret;
}

int test_2(BayCatI2CIO &i2c) {
  uint8_t reg_data[] = {0x00};
  const auto ret = BME680IO::readRegI2C(0x75, reg_data, 1, &i2c);
  std::cout << std::hex << "reg_data: 0x" << static_cast<int>(reg_data[0]) << std::dec << std::endl;
  reg_data[0] = 0x8;
  const auto ret2 = BME280IO::writeRegI2C(0x75, reg_data, 1, &i2c);
  const auto ret3 = BME680IO::readRegI2C(0x75, reg_data, 1, &i2c);
  std::cout << std::hex << "reg_data: 0x" << static_cast<int>(reg_data[0]) << std::dec << std::endl;
  return ret;
}

