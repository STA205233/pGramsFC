#include "BayCatSPIIO.hh"
#include "DAC121S101IO.hh"
#include "FT232HIO.hh"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
using namespace gramsballoon::pgrams;
int main(int argc, char *argv[]) {
  DAC121S101IO dac;
  std::unique_ptr<SPIInterface> spiInterface = nullptr;
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <FT232H or Baycat>" << std::endl;
    return -1;
  }
  std::string interfaceType = argv[1];
  if (interfaceType == "Baycat") {
    std::cout << "Using BayCatSPIIO interface" << std::endl;
    spiInterface = std::make_shared<BayCatSPIIO>();
  }
  else if (interfaceType == "FT232H") {
    std::cout << "Using FT232HIO interface" << std::endl;
    spiInterface = std::make_shared<FT232HIO>();
  }
  else {
    std::cerr << "Invalid interface type: " << interfaceType << ". Use 'FT232H' or 'Baycat'." << std::endl;
    return -1;
  }
  dac.setSPIInterface(spiInterface.get());
  dac.setCS(0);
  spiInterface->setBaudrate(1000000);
  spiInterface->setConfigOptions(FT232HIO::config::SPI_MODE0);
  spiInterface->updateSetting();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  dac.setOperationMode(DAC121S101Mode::DAC121S101_MODE_NORMAL);
  dac.setVoltage(1.0f);
  const auto applyStatus = dac.applySetting();
  if (applyStatus != 0) {
    std::cerr << "Failed to apply DAC setting. Status: " << applyStatus << std::endl;
    return applyStatus;
  }
  std::cout << "Current Voltage: " << dac.getCurrentVoltage() << " V" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  dac.setVoltage(0.0f);
  const auto resetStatus = dac.applySetting();
  if (resetStatus != 0) {
    std::cerr << "Failed to reset DAC voltage. Status: " << resetStatus << std::endl;
    spiInterface->Close();
    return resetStatus;
  }
  std::cout << "Voltage reset to: " << dac.getCurrentVoltage() << " V" << std::endl;
  const int status2 = spiInterface->Close();
  return status2;
}