#include "VL_OSALib.h"
#include <iomanip>
#include <iostream>
int main(int argc, char *argv[]) {
  constexpr unsigned long DIR_GPIO = 0xCA1;
  constexpr unsigned long AUX_OUT = 0xCA3;

  unsigned char direction_char = 0;
  int gpio_id = 0;
  bool direction = true; // Output
  VL_APIStatusT status = VSL_FPGAReadRegister(DIR_GPIO, &direction_char);
  if (status != VL_API_OK) {
    std::cerr << "VSL_FPGAReadRegister failed: " << status << std::endl;
    return -1;
  }
  std::cout << "direction: " << std::bitset<8>(static_cast<int>(direction_char)) << std::endl;
  direction_char |= (1 << gpio_id);
  std::cout << "Setting direction: " << std::bitset<8>(static_cast<int>(direction_char)) << std::endl;

  status = VSL_FPGAWriteRegister(DIR_GPIO, direction_char);
  if (status != VL_API_OK) {
    std::cerr << "VSL_FPGAWriteRegister failed: " << status << std::endl;
    return -1;
  }

  status = VSL_FPGAReadRegister(DIR_GPIO, &direction_char);
  if (status != VL_API_OK) {
    std::cerr << "VSL_FPGAReadRegister failed: " << status << std::endl;
    return -1;
  }
  std::cout << "direction: " << std::bitset<8>(static_cast<int>(direction_char)) << std::endl;
  return 0;
}