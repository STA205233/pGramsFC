#include "SampleTelemetry.hh"
namespace gramsballoon::pgrams {
bool SampleTelemetry::interpret(const std::shared_ptr<BaseTelemetryDefinition> &telemDef) {
  BaseTelemetryDefinition::interpret(telemDef);
  auto contents = getContents();
  if (!contents) {
    return false;
  }
  if (contents->Argc() != 1) {
    return false;
  }
  const auto arg = contents->getArguments(0);
  sampleData_[0] = static_cast<uint8_t>((arg >> 24) & 0xff);
  sampleData_[1] = static_cast<uint8_t>((arg >> 16) & 0xff);
  sampleData_[2] = static_cast<uint8_t>((arg >> 8) & 0xff);
  sampleData_[3] = static_cast<uint8_t>(arg & 0xff);
  return true;
}
void SampleTelemetry::update() {
  auto contents = getContents();
  if (!contents) {
    return;
  }
  std::cout << "HubHKTelemetry::update(): sampleData_ = " << std::endl;
  setArgc(1);
  int32_t arg = (static_cast<int32_t>(sampleData_[0]) << 24) |
                (static_cast<int32_t>(sampleData_[1]) << 16) |
                (static_cast<int32_t>(sampleData_[2]) << 8) |
                (static_cast<int32_t>(sampleData_[3]));
  setArguments(0, arg);
  BaseTelemetryDefinition::update();
}
} // namespace gramsballoon::pgrams
