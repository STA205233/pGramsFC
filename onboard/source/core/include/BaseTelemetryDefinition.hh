#ifndef GRAMSBalloon_BaseTelemetryDefinition_hh
#define GRAMSBalloon_BaseTelemetryDefinition_hh 1
#include "CommunicationFormat.hh"
#include "DBFieldSink.hh"
#include "fstream"
#include <cstdint>
#include <ctime>
#include <memory>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
namespace gramsballoon::pgrams {

enum class Subsystem : uint16_t {
  HUB = 0,
  ORC = 1,
  COL = 2,
  QM = 3,
  TOF = 4,
  UNKNOWN = 255
};
/**
 * @brief A class for base telemetry definition
 * @author Shota Arai
 * @date 2025-xx-xx | Shota Arai | Created
 * @date 2025-12-14 | Shota Arai | Added DB serialization functions
 */
class BaseTelemetryDefinition {
public:
  BaseTelemetryDefinition(bool instantiateContents);
  virtual ~BaseTelemetryDefinition() = default;

private:
  std::shared_ptr<CommunicationFormat> contents_ = nullptr;
  std::time_t timeStamp_ = 0;
  Subsystem subsystem_ = Subsystem::UNKNOWN;
  uint32_t index_ = 0;
  std::unique_ptr<std::regex> reg_ = nullptr;
  std::string content_ = "";
  std::stringstream outss_;
  uint32_t runid_ = 0; // 0 means unset
  bool constructed_ = false;

protected:
  void setArgc(uint16_t argc) {
    if (!contents_) {
      std::cerr << "Contents is not set!" << std::endl;
      return;
    }
    constructed_ = false;
    return contents_->setArgc(argc);
  }
  void setArguments(uint16_t index, int32_t argument) {
    if (!contents_) {
      std::cerr << "Contents is not set!" << std::endl;
      return;
    }
    constructed_ = false;
    return contents_->setArguments(index, argument);
  }
  virtual bool interpret() {
    return true;
  }

public:
  void setRunID(int runId) {
    constructed_ = false;
    runid_ = runId;
  }
  int RunID() const {
    return runid_;
  }
  void setType(Subsystem s) {
    constructed_ = false;
    subsystem_ = s;
  }
  Subsystem getType() const {
    return subsystem_;
  }
  void construct(std::string &outStr);
  void construct();
  bool setContents(const std::shared_ptr<CommunicationFormat> &contents) {
    contents_ = contents;
    constructed_ = false;
    return true;
  }
  const CommunicationFormat *getContents() const { return contents_.get(); }
  CommunicationFormat *getContentsNC() const { return contents_.get(); }
  const std::shared_ptr<CommunicationFormat> &getContentsSptr() const { return contents_; }
  bool parseJSON(const std::string &jsonString);
  void setCurrentTime() {
    timeStamp_ = time(nullptr);
    constructed_ = false;
  }
  void setTimestamp(std::time_t t) {
    timeStamp_ = t;
    constructed_ = false;
  }
  static std::string getTimeString(std::time_t t);
  virtual void update() {
    if (contents_)
      contents_->update();
    constructed_ = false;
  }
  void setIndex(uint32_t index) {
    index_ = index;
    constructed_ = false;
  }
  virtual std::ostream &print(std::ostream &stream) {
    stream << "BaseTelemetryDefinition" << std::endl;
    stream << "Time: " << timeStamp_ << std::endl;
    stream << "Index: " << index_ << std::endl;
    stream << "Subsystem: " << static_cast<int>(subsystem_) << std::endl;
    return contents_->print(stream);
  }
  template <typename Iter, size_t N = 4>
  uint32_t compileValues(Iter iter, Iter end) {
    static_assert(N <= 4 && N > 0);
    uint32_t value = 0;
    for (size_t i = 0; i < N; i++) {
      if (iter == end) {
        break;
      }
      value += (*iter << (8 * (N - 1 - i)));
      iter++;
    }
    return value;
  }
  virtual std::ofstream &write(std::ofstream &stream) {
    if (!constructed_) {
      construct();
    }
    stream.write(outss_.str().c_str(), outss_.str().size());
    return stream;
  }

  /**
   * @brief Initialize DB table structure (setting column names and types...)
   * @param sink DBFieldSink object
   */
  virtual void initializeDBTable(DBFieldSink *sink, const std::string &table_name) const;
  virtual void serialize(DBFieldSink *sink) const;
};
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_BaseTelemetryDefinition_hh