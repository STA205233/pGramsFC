#include "BayCatSPIIO.hh"
#include "FT232HIO.hh"
#include <chrono>
#include <iostream>
#include <thread>
using namespace gramsballoon::pgrams;
int main(int argc, char *argv[]) {
  // Initialize the SPI interface
  SPIInterface *spiInterface = nullptr;
  if (argc == 2 && std::string(argv[1]) == "FT232H") {
    spiInterface = new FT232HIO();
  }
  else {
    spiInterface = new BayCatSPIIO();
  }
  auto ae = spiInterface->Open(0); // Open channel 0
  if (ae != 0) {
    std::cerr << "Error in Open: " << ae << std::endl;
    return -1;
  }
  spiInterface->setConfigOptions(0);
  spiInterface->setBaudrate(1000000); // Set baudrate to 1 MHz
  int status = spiInterface->updateSetting();
  if (status < 0) {
    return 0;
  }
  // Example usage of SPIInterface methods
  uint8_t writeBuffer[] = {0x04, 0x42};
  uint8_t readBuffer[2];
  const auto r = spiInterface->WriteAndRead(0, &writeBuffer[0], 2, &readBuffer[0]);
  if (r < 0) {
    std::cerr << "Error in WriteAndRead: " << r << std::endl;
  }
  // Print the read buffer
  for (int i = 0; i < sizeof(readBuffer); ++i) {
    std::cout << "Read Buffer[" << i << "]: " << static_cast<int>(readBuffer[i]) << std::endl;
  }
  delete spiInterface;
  return 0;
}