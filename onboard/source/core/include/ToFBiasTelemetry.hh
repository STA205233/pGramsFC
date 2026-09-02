#ifndef GB_ToFBiasTelemetry_hh
#define GB_ToFBiasTelemetry_hh 1
#include "BaseTelemetryDefinition.hh"
#include "DBFieldSink.hh"
#include <fstream>
#include <ostream>
#include <string>
namespace gramsballoon::pgrams {
class ToFBiasTelemetry final: public BaseTelemetryDefinition {
public:
  ToFBiasTelemetry(bool instantiateContents);
  ~ToFBiasTelemetry() = default;

public:
  std::ostream &print(std::ostream &stream) override;
  std::ofstream &write(std::ofstream &stream) override;
  bool interpret() override;

  void setArguments(const std::string &str);
  void initializeDBTable(DBFieldSink *sink, const std::string &table_name) const override;
  void serialize(DBFieldSink *sink) const override;
  void reset() override;

private:
  std::string cache_;
};
} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasTelemetry_hh