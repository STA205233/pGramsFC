#include "ADC128S102IO.hh"
#include "BayCatSPIIO.hh"
#include "FT232HIO.hh"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
using namespace gramsballoon::pgrams;

int main(int argc, char *argv[]) {
  ADC128S102IO adc;
  std::unique_ptr<SPIInterface> spiInterface = nullptr;
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <FT232H or Baycat>" << std::endl;
    return -1;
  }
  std::string interfaceType = argv[1];
  if (interfaceType == "Baycat") {
    std::cout << "Using BayCatSPIIO interface" << std::endl;
    spiInterface = std::make_unique<BayCatSPIIO>();
    spiInterface->setConfigOptions(BayCatSPIIO::MakeOption(2, 0)); // SPI mode 2 and MSB first
  }
  else if (interfaceType == "FT232H") {
    std::cout << "Using FT232HIO interface" << std::endl;
    spiInterface = std::make_unique<FT232HIO>();
    spiInterface->setConfigOptions(2);
  }
  else {
    std::cerr << "Invalid interface type: " << interfaceType << ". Use 'FT232H' or 'Baycat'." << std::endl;
    return -1;
  }
  spiInterface->Open(0);
  adc.setSPIInterface(spiInterface.get());
  adc.setCS(0);
  spiInterface->setBaudrate(1000000);
  const auto value = adc.getCurrentVoltage(0);
  if (adc.isError()) {
    std::cerr << "Failed to read ADC channel 0. Error code: " << adc.getErrorCode() << std::endl;
    return adc.getErrorCode();
  }
  std::cout << "Channel 0 Voltage: " << value << " V" << std::endl;
  const int status2 = spiInterface->Close();
  return status2;
}