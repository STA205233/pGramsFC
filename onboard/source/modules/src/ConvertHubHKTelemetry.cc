#include "ConvertHubHKTelemetry.hh"
#include "ConvertedHubHKTelemetry.hh"
#include "HubHKTelemetry.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus ConvertHubHKTelemetry::mod_define() {
  define_parameter("InterpretTelemetry_name", &mod_class::HubHKInterpreterName_);
  define_parameter("verbose", &mod_class::verbose_);
  return AS_OK;
}

ANLStatus ConvertHubHKTelemetry::mod_initialize() {
  if (exist_module(HubHKInterpreterName_)) {
    get_module(HubHKInterpreterName_, &interpretTelemetry_);
  }
  else {
    std::cerr << module_id() << ": Module " << HubHKInterpreterName_ << " not found" << std::endl;
    return AS_QUIT_ALL_ERROR;
  }
  rawTelemetry_ = std::dynamic_pointer_cast<HubHKTelemetry>(interpretTelemetry_->getTelemetryShared());
  if (!rawTelemetry_) {
    std::cerr << module_id() << ": telemetry is not HubHKTelemetry" << std::endl;
    return AS_QUIT_ALL_ERROR;
  }
  convertedTelemetry_ = std::make_shared<ConvertedHubHKTelemetry>();
  return AS_OK;
}

ANLStatus ConvertHubHKTelemetry::mod_analyze() {
  if (interpretTelemetry_->CurrentTelemetryType() == 1) { // HK
    const bool result = convertedTelemetry_->convert(rawTelemetry_.get());
    if (verbose_ > 3) {
      convertedTelemetry_->print(std::cout);
    }
    if (!result) {
      return AS_ERROR;
    }
  }
  return AS_OK;
}
} // namespace gramsballoon::pgrams
