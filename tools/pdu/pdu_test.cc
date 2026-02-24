#include "DAC121S101IO.hh"
#include "FT232HIO.hh"
#include <chrono>
#include <thread>
using namespace gramsballoon::pgrams;
int main() {
  DAC121S101IO dac;
  FT232HIO ft232h;
  ft232h.setBaudrate(1000000);
  ft232h.setConfigOptions(FT232HIO::config::SPI_MODE0);
  const auto status = ft232h.Open(0);
  if (status != 0) {
    std::cerr << "Failed to open FT232H device. Status: " << status << std::endl;
    return status;
  }

  dac.setSPIInterface(&ft232h);
  dac.setCS(0);
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
    ft232h.Close();
    return resetStatus;
  }
  std::cout << "Voltage reset to: " << dac.getCurrentVoltage() << " V" << std::endl;
  const int status2 = ft232h.Close();
  return status2;
}