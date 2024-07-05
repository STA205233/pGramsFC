#include "MeasureTemperatureWithRTDSensor.hh"

using namespace anlnext;
namespace gramsballoon {
ANLStatus MeasureTemperatureWithRTDSensor::mod_define() {
  GBBasicDemoModule::mod_define();
  define_parameter("chatter", &mod_class::chatter_);
  define_parameter("mean_temperature", &mod_class::meanTemperature_);
  define_parameter("temperature_width", &mod_class::temperatureWidth_);
  define_parameter("chip_select", &mod_class::chipSelect_);
  define_parameter("SPIManager_name", &mod_class::SPIManagerName_);
  return AS_OK;
}
void MeasureTemperatureWithRTDSensor::setDataAquisitionError() {
  if (sendTelemetry_ == nullptr) return;

  const std::string name = module_id();
  if (name == "MeasureTemperatureWithRTDSensor_1") {
    sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_DATA_AQUISITION_ERROR_1);
  }
  else if (name == "MeasureTemperatureWithRTDSensor_2") {
    sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_DATA_AQUISITION_ERROR_2);
  }
  else if (name == "MeasureTemperatureWithRTDSensor_3") {
    sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_DATA_AQUISITION_ERROR_3);
  }
  else if (name == "MeasureTemperatureWithRTDSensor_4") {
    sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_DATA_AQUISITION_ERROR_4);
  }
  else if (name == "MeasureTemperatureWithRTDSensor_5") {
    sendTelemetry_->getErrorManager()->setError(ErrorType::RTD_DATA_AQUISITION_ERROR_5);
  }
  else {
    sendTelemetry_->getErrorManager()->setError(ErrorType::OTHER_ERRORS);
  }
}
ANLStatus MeasureTemperatureWithRTDSensor::mod_initialize() {
  meanTemperatureADC_ = TInverseConversion(meanTemperature_);
  temperatureWidthADC_ = TInverseConversion(temperatureWidth_);
  GBBasicDemoModule::mod_initialize();
  return AS_OK;
}
ANLStatus MeasureTemperatureWithRTDSensor::mod_analyze() {
  temperatureADC_ = static_cast<int>(SampleFromUniformDistribution() * temperatureWidthADC_ + meanTemperatureADC_);
  temperature_ = TConversion(temperatureADC_);
  if (chatter_ >= 1) {
    std::cout << "temperature ADC: " << temperatureADC_ << std::endl;
    std::cout << " temperature : " << temperature_ << std::endl;
  }
  return AS_OK;
}

double MeasureTemperatureWithRTDSensor::TConversion(int adc) const {
  const double res = (static_cast<double>(adc) * static_cast<double>(rRef_) / 400.0) / 32.0 - 256.0;
  return res;
}
int MeasureTemperatureWithRTDSensor::TInverseConversion(double temperature) const {
  const int res = static_cast<int>((temperature + 256.0) * 32.0 * 400.0 / static_cast<double>(rRef_));
  return res;
}
} // namespace gramsballoon