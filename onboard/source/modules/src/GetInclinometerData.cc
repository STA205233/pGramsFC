#include "GetInclinometerData.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus GetInclinometerData::mod_define() {
  define_parameter("SerialPath", &mod_class::serialPath_);
  set_parameter_description("Serial path to the inclinometer");
  define_parameter("Baudrate", &mod_class::baudrate_);
  set_parameter_description("Baudrate for the serial communication");
  define_parameter("timeout_sec", &mod_class::timeoutSec_);
  set_parameter_description("Timeout seconds for the serial communication");
  define_parameter("timeout_usec", &mod_class::timeoutUsec_);
  set_parameter_description("Timeout microseconds for the serial communication");
  define_parameter("Chatter", &mod_class::chatter_);
  set_parameter_description("Chatter level");
  return AS_OK;
}
ANLStatus GetInclinometerData::mod_initialize() {
  if (exist_module("SendTelemetry")) {
    get_module_NC("SendTelemetry", &sendTelemetry_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize SendTelemetry module is not found." << std::endl;
  }
  inclinometerController_ = std::make_shared<InclinometerController>(serialPath_, baudrate_);
  if (!inclinometerController_) {
    std::cerr << module_id() << "::mod_initialize Failed to create InclinometerController." << std::endl;
    return AS_ERROR;
  }
  const int status = inclinometerController_->initialize();
  if (status < 0) {
    std::cerr << module_id() << "::mod_initialize Failed to initialize InclinometerController." << std::endl;
    if (sendTelemetry_) {
      //sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_SERIAL_COMMUNICATION_ERROR);
    }
    return AS_ERROR;
  }
}
ANLStatus GetInclinometerData::mod_analyze() {
  int32_t x = 0, y = 0;
  const int status_xy = inclinometerController_->getXY(x, y);
  if (status_xy < 0) {
    std::cerr << module_id() << "::mod_analyze Failed to get XY data from InclinometerController." << std::endl;
    if (sendTelemetry_) {
      //sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_SERIAL_COMMUNICATION_ERROR);
    }
    return AS_ERROR;
  }
  int16_t temp = 0;
  const int status_temp = inclinometerController_->getTemp(temp);
  if (status_temp < 0) {
    std::cerr << module_id() << "::mod_analyze Failed to get temperature data from InclinometerController." << std::endl;
    if (sendTelemetry_) {
      //sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_SERIAL_COMMUNICATION_ERROR);
    }
    return AS_ERROR;
  }
  if (chatter_ > 0) {
    std::cout << "Inclinometer Data - X: " << x << ", Y: " << y << ", Temp: " << temp << std::endl;
  }
  return AS_OK;
}
ANLStatus GetInclinometerData::mod_finalize() {
  inclinometerController_.reset();
  return AS_OK;
}
} // namespace gramsballoon::pgrams