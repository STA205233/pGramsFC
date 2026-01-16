#include "GetMHADCData.hh"
#include "boost/format.hpp"
#include <chrono>
#include <thread>
using namespace anlnext;

namespace gramsballoon::pgrams {
ANLStatus GetMHADCData::mod_define() {
  define_parameter("num_section", &mod_class::numSection_);
  define_parameter("channel_per_section", &mod_class::channelPerSection_);
  define_parameter("MHADCManager_name", &mod_class::encodedSerialCommunicatorName_);
  define_parameter("sleep_for_msec", &mod_class::sleepForMsec_);
  define_parameter("num_trials", &mod_class::numTrials_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus GetMHADCData::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  numCh_ = numSection_ * channelPerSection_;
  adcData_.resize(numCh_);
  if (chatter_ > 0) {
    std::cout << "Pattern: " << std::endl;
  }
  for (int i = 0; i < numSection_; i++) {
    const char section_name = 'A' + i;
    for (int j = 1; j <= channelPerSection_; j++) {
      std::string pat = (boost::format("%c%d_(\\d*)") % section_name % j).str();
      if (chatter_ > 0) {
        std::cout << pat << " ";
      }
      regs_.push_back(std::regex(pat));
    }
    if (chatter_ > 0){
      std::cout << std::endl;
    } 
  }
  if (exist_module(encodedSerialCommunicatorName_)) {
    get_module_NC(encodedSerialCommunicatorName_, &encodedSerialCommunicator_);
  }
  else {
    std::cerr << encodedSerialCommunicatorName_ << " does not exist." << std::endl;
    encodedSerialCommunicator_ = nullptr;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
    return AS_ERROR;
  }
  return AS_OK;
}

ANLStatus GetMHADCData::mod_analyze() {
  if (chatter_ > 0) {
    std::cout << "GetMHADCData::mod_analyze" << std::endl;
  }
  std::vector<bool> failed_ch(numCh_, false);
  adcData_.resize(numCh_, 0);
  std::string dat;
  if (!encodedSerialCommunicator_) {
    return AS_OK;
  }
  for (int j = 0; j < numTrials_; j++) {
    const int byte_read = encodedSerialCommunicator_->SendComAndGetData("a", dat, sleepForMsec_);
    if (byte_read < 0) {
      std::cerr << "Error in GetMHADCData::mod_analyze: byte_read = " << byte_read << std::endl;
      if (sendTelemetry_) {
        sendTelemetry_->getErrorManager()->setError(ErrorType::MHADC_COM_ERROR);
      }
      for (int i = 0; i < numCh_; i++) {
        failed_ch[i] = true;
      }
      continue;
    }
    else if (byte_read == 0) {
      continue;
    }
    if (chatter_ > 1) {
      std::cout << "Received message: " << dat << std::endl;
    }
    bool success = true;
    for (int i = 0; i < numCh_; i++) {
      std::smatch m;
      std::regex_search(dat, m, regs_[i]);
      try {
        adcData_[i] = std::stoi(m[1].str());
        failed_ch[i] = false;
      }
      catch (const std::invalid_argument &e) {
        std::cerr << "Ch " << i << " data cannot be converted." << std::endl;
        std::cerr << "Data: " << dat << std::endl;
        adcData_[i] = 0;
        failed_ch[i] = true;
        success = false;
      }
      if (chatter_ > 0) {
        std::cout << "Ch" << i << ": " << adcData_[i] << std::endl;
      }
    }
    if (success) {
      break;
    }
    else {
      for (int i = 0; i < numCh_; i++) {
        if (failed_ch[i]) {
          //sendTelemetry_->getErrorManager()->setError(ConvertRTDError(i)); // TODO: Define MHADC error
        }
      }
    }
  }
  return AS_OK;
}
ANLStatus GetMHADCData::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams