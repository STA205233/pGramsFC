#ifndef GRAMSBalloon_HubHKTelemetry_HH
#define GRAMSBalloon_HubHKTelemetry_HH 1
#include "BaseTelemetryDefinition.hh"
namespace gramsballoon::pgrams {
class SampleTelemetry: public BaseTelemetryDefinition {
public:
  SampleTelemetry() : BaseTelemetryDefinition() {
    setType(Subsystem::HUB);
  }
  virtual ~SampleTelemetry() = default;

private:
  std::array<uint8_t, 4> sampleData_ = {0x00, 0x22, 0x01, 0x12};

public:
  bool interpret(const std::shared_ptr<BaseTelemetryDefinition> &telemDef) override;
  void update() override;
  std::ostream &print(std::ostream &stream) override {
    stream << "HubHKTelemetry" << std::endl;
    stream << "Sample Data: ";
    for (const auto &d: sampleData_) {
      stream << static_cast<int>(d) << " ";
    }
    stream << std::endl;
    BaseTelemetryDefinition::print(stream);
    return stream;
  }
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_HubHKTelemetry_HH