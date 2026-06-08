#include "ADC128S102IO.hh"
#include "BayCatSPIIO.hh"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
using namespace gramsballoon::pgrams;

int main(int argc, char *argv[]) {
  ADC128S102IO adc;
  auto spiInterface = std::make_unique<BayCatSPIIO>();
  spiInterface->Open(0);
  adc.setSPIInterface(spiInterface.get());
  adc.setCS(0);
  spiInterface->setBaudrate(1000000);
  spiInterface->setConfigOptions(BayCatSPIIO::MakeOption(2, 0)); // SPI mode 2 and MSB first
  std::this_thread::sleep_for(std::chrono::seconds(5));
  const auto status = adc.getCurrentVoltage(0);
  if (status != 0) {
    std::cerr << "Failed to read ADC channels. Status: " << status << std::endl;
    return status;
  }
  if (adc.isError()) {
    std::cerr << "Failed to read ADC channel 0. Error code: " << adc.getErrorCode() << std::endl;
    return adc.getErrorCode();
  }
  std::cout << "Channel 0 Voltage: " << status << " V" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  const int status2 = spiInterface->Close();
  return status2;
}