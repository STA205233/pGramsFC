#include "MeasureOrientationByMHADC.hh"
#include <iostream>
using namespace anlnext;

namespace gramsballoon::pgrams {
ANLStatus MeasureOrientationByMHADC::mod_define() {
  define_parameter("encodedSerialCommunicator_name", &mod_class::encodedSerialCommunicatorName_);
  define_parameter("sleep_for_msec", &mod_class::sleepForMsec_);
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus MeasureOrientationByMHADC::mod_initialize() {
  orientationInfo_ = std::make_shared<OrientationInformation>();
  orientationInfo_->SetChatter(chatter_);
  if (exist_module(encodedSerialCommunicatorName_)) {
    get_module_NC(encodedSerialCommunicatorName_, &esc_);
  }
  else {
    std::cerr << "MeasureOrientationByMHADC::mod_initialize: Module(" << encodedSerialCommunicatorName_ << ") is not found" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::MODULE_ACCESS_ERROR);
    }
    return AS_ERROR;
  }
  return AS_OK;
}

ANLStatus MeasureOrientationByMHADC::mod_analyze() {
  if (!esc_) {
    std::cerr << "MeasureOrientationByMHADC::mod_analyze: EncodedSerialCommunicator is not found" << std::endl;
    return AS_OK;
  }
  if (!orientationInfo_) {
    std::cerr << "MeasureOrientaionByMHADC::mod_analyze: OrientationInformation is nullptr" << std::endl;
    return AS_OK;
  }
  std::string data;
  for (int i = 0; i < numTrials_; i++) {
    const int byte = esc_->SendComAndGetData("s", data, sleepForMsec_);
    if (byte < 0) {
      std::cerr << "MeasureOrientationByMHADC::mod_analyze: SendComAndGetData failed" << std::endl;
      if (sendTelemetry_) {
        sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_SERIAL_COMMUNICATION_ERROR);
      }
      continue;
    }
    else if (byte == 0) {
      continue;
    }
    const bool ret = orientationInfo_->SetString(data);
    if (!ret) {
      std::cerr << "MeasureOrientationByMHADC::mod_analyze: Interpretation of orientation data failed.\nstring: " << data << std::endl;
      continue;
    }
    break;
  }

  return AS_OK;
}
ANLStatus MeasureOrientationByMHADC::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams