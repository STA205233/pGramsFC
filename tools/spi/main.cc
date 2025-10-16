#include "BayCatControl.hh"
#include "FT232HIO.hh"
#include <iostream>
#include <thread>
#include <chrono>
using namespace gramsballoon::pgrams;
int main(int argc, char *argv[]) {
  // Initialize the SPI interface
  //Init_libMPSSE();
  //FT232HIO spiInterface;
  BayCatControl spiInterface;
  //spiInterface.setBaudrate(1000000); // Set baudrate to 1 MHz
  //spiInterface.setConfigOptions(SPI_CONFIG_OPTION_CS_ACTIVELOW | SPI_CONFIG_OPTION_MODE1 | SPI_CONFIG_OPTION_CS_DBUS3);
  spiInterface.setConfigOptions(3);
  auto ae = spiInterface.Open(0); // Open channel 0
  if (ae != 0) {
    std::cerr << "Error in Open: " << ae << std::endl;
    return -1;
  }
  //spiInterface.controlGPIO(17, true);
  // Example usage of SPIInterface methods
  //uint8_t writeBuffer = {0x03};
  //uint8_t readBuffer[1];
  //const auto r = spiInterface.WriteAfterRead(0, &writeBuffer, 1, &readBuffer[0], 1);
  //if (r != 0) {
  //  std::cerr << "Error in WriteAndRead: " << r << std::endl;
  //}
  //// Print the read buffer
  //for (int i = 0; i < sizeof(readBuffer); ++i) {
  //  std::cout << "Read Buffer[" << i << "]: " << static_cast<int>(readBuffer[i]) << std::endl;
  //}
  //uint8_t writeBuffer2[] = {0x83, 0x06};
  unsigned char a = 0;
  spiInterface.controlGPIO(17, true);
  std::this_thread::sleep_for(std::chrono::microseconds(100));
  spiInterface.controlGPIO(17, true);
  //spiInterface.ReadFPGARegister(0xC80, &a);
  //std::cout << static_cast<int>(a&0b1111111) << std::endl;
  //spiInterface.controlGPIO(17, false);
  //const auto ff = spiInterface.Write(0, writeBuffer2, 2);
  //if (ff != 0) {
  //  std::cerr << "Error in Write: " << ff << std::endl;
  //}
  //const auto  r2 = spiInterface.WriteAfterRead(0, &writeBuffer, 1, &readBuffer[0], 1);
  //if (r2 != 0) {
  //  std::cerr << "Error in WriteAndRead: " << r2 << std::endl;
  //}
  //// Print the read buffer
  //for (int i = 0; i < sizeof(readBuffer); ++i) {
  //  std::cout << "Read Buffer[" << i << "]: " << static_cast<int>(readBuffer[i]) << std::endl;
  //}
  //// Close the SPI interface
  //auto  dadsf = spiInterface.Close();
  //if (dadsf != 0) {
  //  std::cerr << "Error in Close: " << dadsf << std::endl;
  //}
  //Cleanup_libMPSSE();
  return 0;
}