#include "SPIInterface.hh"
#include <iostream>
using namespace gramsballoon::pgrams;
int main(int argc, char *argv[]) {
  // Initialize the SPI interface
  Init_libMPSSE();
  SPIInterface spiInterface;
  spiInterface.setBaudrate(1000000); // Set baudrate to 1 MHz
  spiInterface.setConfigOptions(SPI_CONFIG_OPTION_CS_ACTIVELOW | SPI_CONFIG_OPTION_MODE1 | SPI_CONFIG_OPTION_CS_DBUS3);
  FT_STATUS ae = spiInterface.Open(0); // Open channel 0
  if (ae != FT_OK) {
    std::cerr << "Error in Open: " << ae << std::endl;
    return -1;
  }

  // Example usage of SPIInterface methods
  uint8_t writeBuffer = {0x03};
  uint8_t readBuffer[1];
  const FT_STATUS r = spiInterface.WriteAndRead(8, &writeBuffer, sizeof(writeBuffer), readBuffer, 1);
  if (r != FT_OK) {
    std::cerr << "Error in WriteAndRead: " << r << std::endl;
  }
  // Print the read buffer
  for (int i = 0; i < sizeof(readBuffer); ++i) {
    std::cout << "Read Buffer[" << i << "]: " << static_cast<int>(readBuffer[i]) << std::endl;
  }
  uint8_t writeBuffer2[] = {0x83, 0x06};

  const FT_STATUS ff = spiInterface.Write(8, writeBuffer2, 2);
  if (ff != FT_OK) {
    std::cerr << "Error in Write: " << ff << std::endl;
  }
  const FT_STATUS r2 = spiInterface.WriteAndRead(8, &writeBuffer, sizeof(writeBuffer), readBuffer, 1);
  if (r2 != FT_OK) {
    std::cerr << "Error in WriteAndRead: " << r2 << std::endl;
  }
  // Print the read buffer
  for (int i = 0; i < sizeof(readBuffer); ++i) {
    std::cout << "Read Buffer[" << i << "]: " << static_cast<int>(readBuffer[i]) << std::endl;
  }
  // Close the SPI interface
  FT_STATUS dadsf = spiInterface.Close();
  if (dadsf != FT_OK) {
    std::cerr << "Error in Close: " << dadsf << std::endl;
  }
  Cleanup_libMPSSE();
  return 0;
}