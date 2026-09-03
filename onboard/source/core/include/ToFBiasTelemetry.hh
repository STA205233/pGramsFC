#ifndef GB_ToFBiasTelemetry_hh
#define GB_ToFBiasTelemetry_hh 1
#include "BaseTelemetryDefinition.hh"
#include "DBFieldSink.hh"
#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
namespace gramsballoon::pgrams {
class ToFBiasTelemetry final: public BaseTelemetryDefinition {
public:
  static constexpr std::size_t LENGTH_SUMMARY = 538;
  ToFBiasTelemetry(bool instantiateContents);
  ~ToFBiasTelemetry() = default;

public:
  std::ostream &print(std::ostream &stream) override;
  std::ofstream &write(std::ofstream &stream) override;
  bool interpret() override;

  void setArguments(const std::string &str);
  void initializeDBTable(DBFieldSink *sink, const std::string &table_name) const override { return initializeDBTableFull(sink, table_name); }
  void initializeDBTableFull(DBFieldSink *sink, const std::string &table_name) const;
  void initializeDBTableSummary(DBFieldSink *sink, const std::string &table_name) const;
  void serialize(DBFieldSink *sink) const override;
  void reset() override;

  enum class MODE {
    AUTO,
    DEBUG,
    UNKNOWN
  };

private:
  bool interpretSummaryOutput();

  std::string cache_;
  uint8_t cardType_ = 0;
  uint8_t frameCounter_ = 0;
  MODE mode_ = MODE::UNKNOWN;
  std::array<uint8_t, 8> hvEnableSetting_{};
  std::array<uint16_t, 16> tmonReading_{};
  std::array<uint16_t, 16> vmonReading_{};
  std::array<uint16_t, 128> trimDACSetting_{};
  std::array<uint16_t, 2> boardTmonReading_{};
};
} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasTelemetry_hh