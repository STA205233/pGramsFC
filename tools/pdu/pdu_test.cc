#include "BayCatSPIIO.hh"
#include "DAC121S101IO.hh"
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
  DAC121S101IO dac;
  std::shared_ptr<SPIInterface> spiInterface2 = nullptr;
  std::unique_ptr<SPIInterfaceMultiplexer> spiInterface = std::make_unique<SPIInterfaceMultiplexer>();
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <FT232H, MCP2210 or Baycat>" << std::endl;
    return -1;
  }
  std::string interfaceType = argv[1];
  std::string path = "";
  if (interfaceType == "Baycat") {
    std::cout << "Using BayCatSPIIO interface" << std::endl;
    spiInterface2 = std::make_shared<BayCatSPIIO>();
    spiInterface2->setConfigOptions(BayCatSPIIO::MakeOption(1, 0)); // SPI mode 1 and MSB first
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
    std::cerr << "Invalid interface type: " << interfaceType << ". Use 'FT232H' or 'Baycat', 'MCP2210'." << std::endl;
    return -1;
  }
  spiInterface2->Open(0, path.c_str());
  spiInterface->setBaseInterface(spiInterface2);
  spiInterface->setMappingChipSelect(std::make_unique<PDUCSMapping>(0x1f0000));
  dac.setSPIInterface(spiInterface.get());

  for (int i = 10; i < 17; ++i) {
    dac.setCS(i); // Chip select
    std::this_thread::sleep_for(std::chrono::seconds(1));
    dac.setOperationMode(DAC121S101Mode::DAC121S101_MODE_NORMAL);
    dac.setVoltage(3.0f); // voltage control
    const auto applyStatus = dac.applySetting();
    if (applyStatus != 0) {
      std::cerr << "Failed to apply DAC setting. Status: " << applyStatus << std::endl;
      return applyStatus;
    }
    std::cout << "Current Voltage: " << dac.getCurrentVoltage() << " V" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    dac.setVoltage(0.0f);
    //dac.setOperationMode(DAC121S101Mode::DAC121S101_MODE_PowerDown_HiZ);
    const auto resetStatus = dac.applySetting();
    if (resetStatus != 0) {
      std::cerr << "Failed to reset DAC voltage. Status: " << resetStatus << std::endl;
      spiInterface->Close();
      return resetStatus;
    }
    std::cout << "Voltage reset to: " << dac.getCurrentVoltage() << " V" << std::endl;
  }
  const int status2 = spiInterface->Close();
  return status2;
}
