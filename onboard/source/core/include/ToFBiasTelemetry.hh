#ifndef GB_ToFBiasTelemetry_hh
#define GB_ToFBiasTelemetry_hh 1
#include "BaseTelemetryDefinition.hh"
namespace gramsballoon::pgrams {
class ToFBiasTelemetry final: public BaseTelemetryDefinition {
public:
  ToFBiasTelemetry(bool instantiateContents);
  ~ToFBiasTelemetry() = default;

  void setArguments(const std::string &str);

public:
  std::ostream &print(std::ostream &stream) override;
  std::ofstream &write(std::ofstream &stream) override;
  bool toStr(std::string &str) const;
};
} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasTelemetry_hh