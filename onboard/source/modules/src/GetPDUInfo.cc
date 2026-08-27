#include "GetPDUInfo.hh"
#include "ADC128S102IO.hh"
#include "SPIManager.hh"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus GetPDUInfo::mod_define() {
  define_parameter("SPIManager_name", &mod_class::spiManagerName_);
  set_parameter_description("SPIManager name for accessing SPI interface");
  define_parameter("v_ref", &mod_class::vref_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus GetPDUInfo::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  if (exist_module(spiManagerName_)) {
    get_module_NC(spiManagerName_, &spiManager_);
  }
  else {
    std::cerr << "GetPDUInfo: SPIManager module not found: " << spiManagerName_ << std::endl;
    return AS_ERROR;
  }
  auto interface = spiManager_->Interface();
  if (!interface) {
    std::cerr << "GetPDUInfo: SPI interface is not initialized" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::SPI_OPEN_ERROR);
    }
    return AS_ERROR;
  }
  adc_ = std::make_shared<ADC128S102IO>(vref_);
  adc_->setSPIInterface(interface);
  voltages_.resize(numAdcs_ * ADC128S102IO::MaxChannelsPerADC());
  lastMonitorTime_ = std::chrono::steady_clock::now(); // [MONITOR]
  return AS_OK;
}
ANLStatus GetPDUInfo::mod_analyze() {
  const auto analyze_start = std::chrono::steady_clock::now(); // [MONITOR]
  if (!adc_) {
    return AS_ERROR;
  }
  if (!adc_->isSPIInterfaceSet()) {
    if (!spiManager_) {
      return AS_OK;
    }
    auto interface = spiManager_->Interface();
    if (!interface) {
      std::cerr << "GetPDUInfo: SPI interface is not initialized" << std::endl;
      return AS_ERROR;
    }
    adc_->setSPIInterface(interface);
  }

  for (int cs = 0; cs < numAdcs_; ++cs) {
    adc_->setCS(cs);
    for (size_t ch = 0; ch < ADC128S102IO::MaxChannelsPerADC(); ++ch) {
      const uint16_t voltage = adc_->getCurrentVoltageADC(ch);
      if (chatter_ > 2) {
        std::cout << "PDU Channel " << ch << " (CS " << cs << "): " << adc_->convertVoltage(voltage) << " V" << std::endl;
      }
      setVoltage(cs, ch, voltage);
      if (adc_->isError()) {
        std::cerr << "Error reading voltage for CS " << cs << ", channel " << ch << ": " << adc_->getErrorCode() << std::endl;
        if (sendTelemetry_) {
          sendTelemetry_->getErrorManager()->setError(ErrorType::ADC_ACCESS_ERROR);
        }
      }
    }
  }
  // [MONITOR]
  const auto now = std::chrono::steady_clock::now();
  elapsedSumInInterval_ += std::chrono::duration<double, std::milli>(now - analyze_start).count();
  numAnalyzeInInterval_++;
  if (now - lastMonitorTime_ >= std::chrono::seconds(1)) {
    const double interval_sec = std::chrono::duration<double>(now - lastMonitorTime_).count();
    std::cout << "[MONITOR] " << module_id()
              << " called=" << numAnalyzeInInterval_ / interval_sec << " /s"
              << " analyze=" << elapsedSumInInterval_ / numAnalyzeInInterval_ << " ms"
              << std::endl;
    lastMonitorTime_ = now;
    elapsedSumInInterval_ = 0.0;
    numAnalyzeInInterval_ = 0;
  }
  return AS_OK;
}

void GetPDUInfo::setVoltage(int cs, int ch, uint16_t value) {
  const size_t index = CalIndex(cs, ch);
  if (index > voltages_.size()) {
    return;
  }
  voltages_[index] = value;
}
} // namespace gramsballoon::pgrams