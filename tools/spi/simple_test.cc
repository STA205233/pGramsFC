#define linux 1
#include "stdbool.h"
#include "VL_OSALib.h"
#include <iomanip>
#include <iostream>
#include <bitset>
int main(int argc, char *argv[]) {
  constexpr unsigned long DIR_GPIO = 0xCA1;
  constexpr unsigned long AUX_OUT = 0xCA3;
  VSL_DebugLevelSet(2);
  auto a = VSL_Open();
  if (a != 0){
    std::cout << "VSL_OpenError: " << a << std::endl;
    return -1;
  }
  unsigned char board_name[1000];
  unsigned char  Attribute, WDT, BIOSInfo;
  short DIOS, Timers, AIn, AOut, Serial, FanSupport;
  VSL_GetProductInfo(1, &board_name[0], &Attribute, &DIOS, &Timers, &WDT, &AIn, &AOut, &Serial, &FanSupport, &BIOSInfo);
  std::cout << "Board Name: " << board_name << std::endl;
  std::cout << "DIOs: " << DIOS << std::endl;
  std::cout << "Timers: " << Timers << std::endl;
  std::cout << "AIn: " << AIn << std::endl;
  std::cout << "AOut: " << AOut << std::endl;
  std::cout << "Serial: " << Serial << std::endl;
  std::cout << "FanSupport: " << FanSupport << std::endl;
  //unsigned char direction_char = 0;
  
  //bool direction = true; // Output
  //unsigned char mode = 0;
  //{auto status2 = VSL_FPGAReadRegister(0xCA7, &mode);
  //if (status2 != VL_API_OK) {
  //  std::cerr << "VSL_FPGAReadRegister failed: " << status2 << std::endl;
  //  return -1;
  //}}
  
  //std::cout << "mode: " << std::bitset<8>(static_cast<int>(mode)) << std::endl;
  //VSL_FPGAWriteRegister(0xCA7, 0);
  //{auto status2 = VSL_FPGAReadRegister(0xCA7, &mode);
  //if (status2 != VL_API_OK) {
  //  std::cerr << "VSL_FPGAReadRegister failed: " << status2 << std::endl;
  //  return -1;
  //}}
  //std::cout << "mode: " << std::bitset<8>(static_cast<int>(mode)) << std::endl;
  //return -0;
  //VL_APIStatusT status = VSL_DIOGetChannelDirection(gpio_id, &direction_char);
  //if (status != VL_API_OK) {
  //  std::cerr << "VSL_FPGAReadRegister failed: " << status << std::endl;
  //  return -1;
  //}
  //std::cout << "direction: " << std::bitset<8>(static_cast<int>(direction_char)) << std::endl;
  ////std::cout << "Setting direction: " << std::bitset<8>(static_cast<int>(direction_char)) << std::endl;

  //VSL_DIOSetChannelDirection(gpio_id, DIO_OUTPUT);

  //status = VSL_DIOGetChannelDirection(gpio_id, &direction_char);
  //if (status != VL_API_OK) {
  //  std::cerr << "VSL_FPGAReadRegister failed: " << status << std::endl;
  //  return -1;
  //}
  //std::cout << "direction: " << std::bitset<8>(static_cast<int>(direction_char)) << std::endl;
  VSL_Close();
  return 0;
}