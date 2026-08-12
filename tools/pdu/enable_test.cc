#include "BayCatSPIIO.hh"
#include "FT232HIO.hh"
#include "SPIInterfaceMultiplexer.hh"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace gramsballoon::pgrams;

const int chip_select = 0; // PLEASE MODIFY
const bool is_high = true; // If high, please specify true, otherwise false.

int main(int argc, char *argv[]) {
  std::shared_ptr<SPIInterface> spiInterface2 = nullptr;
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <FT232H or Baycat>" << std::endl;
    return -1;
  }
  std::string interfaceType = argv[1];
  if (interfaceType == "Baycat") {
    std::cout << "Using BayCatSPIIO interface" << std::endl;
    spiInterface2 = std::make_shared<BayCatSPIIO>();
    spiInterface2->setConfigOptions(BayCatSPIIO::MakeOption(2, 0)); // SPI mode 2 and MSB first
  }
  else if (interfaceType == "FT232H") {
    std::cout << "Using FT232HIO interface" << std::endl;
    spiInterface2 = std::make_shared<FT232HIO>();
    spiInterface2->setConfigOptions(2);
  }
  else {
    std::cerr << "Invalid interface type: " << interfaceType << ". Use 'FT232H' or 'Baycat'." << std::endl;
    return -1;
  }
  spiInterface2->Open(0);
  spiInterface2->controlGPIO(chip_select, is_high);
  std::this_thread::sleep_for(std::chrono::seconds(5)); // after 5 sec, the process will continue.
  const int status2 = spiInterface2->Close();
  return status2;
}