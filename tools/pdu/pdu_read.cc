#include "ADC128S102IO.hh"
#include "BayCatSPIIO.hh"
#include "FT232HIO.hh"
#include "MCP2210IO.hh"
#include "PDUCSMapping.hh"
#include "SPIInterfaceMultiplexer.hh"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
using namespace gramsballoon::pgrams;

int main(int argc, char *argv[]) {
  ADC128S102IO adc(5.0);
  std::shared_ptr<SPIInterface> spiInterface2 = nullptr;
  std::unique_ptr<SPIInterfaceMultiplexer> spiInterface = std::make_unique<SPIInterfaceMultiplexer>();
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <FT232H, MCP2210 or Baycat>" << std::endl;
    return -1;
  }
  std::string path = "";
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
  else if (interfaceType == "MCP2210") {
    std::cout << "Using MCP2210 interface" << std::endl;
    path = "/dev/hidraw0";
    spiInterface2 = std::make_shared<MCP2210IO>();
    spiInterface2->setConfigOptions(2);
  }
  else {
    std::cerr << "Invalid interface type: " << interfaceType << ". Use 'FT232H', 'Baycat' or 'MCP2210'." << std::endl;
    return -1;
  }
  spiInterface2->Open(0, path.c_str());
  spiInterface->setBaseInterface(spiInterface2);
  spiInterface->setMappingChipSelect(std::make_unique<PDUCSMapping>(0x1f0000));
  adc.setSPIInterface(spiInterface.get());
  spiInterface->setBaudrate(1000000);
  for (int i = 0; i < 10; ++i) {
    adc.setCS(i);
    for (int j = 0; j < 8; ++j) {
      const auto value = adc.getCurrentVoltage(j);
      if (adc.isError()) {
        std::cerr << "Failed to read ADC channel " << j << " . Error code: " << adc.getErrorCode() << std::endl;
        return adc.getErrorCode();
      }
      std::cout << "Channel " << j << " Voltage: " << value << " V" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
  const int status2 = spiInterface->Close();
  return status2;
}