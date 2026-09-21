#ifndef GRAMSBalloon_BaseTelemetryDefinition_hh
#define GRAMSBalloon_BaseTelemetryDefinition_hh 1
#include "CommunicationFormat.hh"
#include "DBFieldSink.hh"
#include "DBSerializable.hh"
#include "fstream"
#include "magic_enum.hpp"
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

std::ostream &operator<<(std::ostream &os, const Subsystem &subsystem);

/**
 * @brief A class for base telemetry definition
 * @author Shota Arai
 * @date 2025-xx-xx | Shota Arai | Created
 * @date 2025-12-14 | Shota Arai | Added DB serialization functions
 * @date 2026-06-14 | Shota Arai | Detached DB serialization functions to DBSerializable
 */
class BaseTelemetryDefinition: public DBSerializable {
public:
  /**
   * @brief Constructor
   * @param[in] instantiateContents Set true to instantiate the CommunicationFormat contents here
   */
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
  /**
   * @brief Set the number of the arguments of the contents
   * @param[in] argc Number of the arguments
   */
  void setArgc(uint16_t argc) {
    if (!contents_) {
      std::cerr << "Contents is not set!" << std::endl;
      return;
    }
    constructed_ = false;
    return contents_->setArgc(argc);
  }
  /**
   * @brief Set one argument of the contents
   * @param[in] index Index of the argument
   * @param[in] argument Value of the argument
   */
  void setArguments(uint16_t index, uint32_t argument) {
    if (!contents_) {
      std::cerr << "Contents is not set!" << std::endl;
      return;
    }
    constructed_ = false;
    return contents_->setArguments(index, argument);
  }
  /**
   * @brief Interpret the arguments of the contents into the member variables of the derived class
   */
  virtual bool interpret() {
    return true;
  }

public:
  /**
   * @brief Clear the contents of the telemetry
   */
  virtual void reset() {
    if (contents_) {
      contents_->setCode(0);
      contents_->setArgc(0);
      contents_->CommandNC().clear();
    }
    constructed_ = false;
  }
  /**
   * @brief Set the run ID of the telemetry
   * @param[in] runId Run ID, 0 means unset
   */
  void setRunID(int runId) {
    constructed_ = false;
    runid_ = runId;
  }
  /**
   * @brief Return the run ID of the telemetry
   */
  int RunID() const {
    return runid_;
  }
  /**
   * @brief Set the subsystem which sends the telemetry
   * @param[in] s Subsystem
   */
  void setType(Subsystem s) {
    constructed_ = false;
    subsystem_ = s;
  }
  /**
   * @brief Return the subsystem which sends the telemetry
   */
  Subsystem getType() const {
    return subsystem_;
  }
  /**
   * @brief Construct the telemetry string and copy it to the given string
   * @param[out] outStr Destination of the constructed telemetry string
   */
  void construct(std::string &outStr);
  /**
   * @brief Construct the telemetry string from the timestamp, the subsystem, the index and the contents
   */
  void construct();
  /**
   * @brief Set the contents of the telemetry
   * @param[in] contents Contents to be held by this telemetry
   */
  bool setContents(const std::shared_ptr<CommunicationFormat> &contents) {
    contents_ = contents;
    constructed_ = false;
    return true;
  }
  /**
   * @brief Return the read-only pointer to the contents
   */
  const CommunicationFormat *getContents() const { return contents_.get(); }
  /**
   * @brief Return the modifiable pointer to the contents
   */
  CommunicationFormat *getContentsNC() const { return contents_.get(); }
  /**
   * @brief Return the shared pointer to the contents
   */
  const std::shared_ptr<CommunicationFormat> &getContentsSptr() const { return contents_; }
  /**
   * @brief Parse a telemetry string in the JSON-like format and restore the contents
   * @param[in] jsonString Telemetry string to be parsed
   */
  bool parseJSON(const std::string &jsonString);
  /**
   * @brief Set the timestamp of the telemetry to the current time
   */
  void setCurrentTime() {
    timeStamp_ = time(nullptr);
    constructed_ = false;
  }
  /**
   * @brief Set the timestamp of the telemetry
   * @param[in] t Timestamp to be set
   */
  void setTimestamp(std::time_t t) {
    timeStamp_ = t;
    constructed_ = false;
  }
  /**
   * @brief Return the timestamp of the telemetry
   */
  std::time_t TimeStamp() const { return timeStamp_; }
  /**
   * @brief Convert a timestamp into the string used in the telemetry
   * @param[in] t Timestamp to be converted
   */
  static std::string getTimeString(std::time_t t);
  /**
   * @brief Convert a time string in the telemetry into a timestamp of the current year
   * @param[in] str Time string to be converted
   */
  static std::time_t ConvertFromTimeString(const std::string &str);
  /**
   * @brief Update the contents, which is expected to be overridden to fill the latest values
   */
  virtual void update() {
    if (contents_)
      contents_->update();
    constructed_ = false;
  }
  /**
   * @brief Set the index of the telemetry
   * @param[in] index Index of the telemetry
   */
  void setIndex(uint32_t index) {
    index_ = index;
    constructed_ = false;
  }

  /**
   * @brief Return the subsystem which sends the telemetry
   */
  Subsystem getSubsystem() const { return subsystem_; }

  /**
   * @brief Print the telemetry in a human-readable format
   * @param[in,out] stream Output stream to be printed to
   */
  virtual std::ostream &print(std::ostream &stream);
  /**
   * @brief Write the constructed telemetry string to a file
   * @param[in,out] stream Output file stream to be written to
   */
  virtual std::ofstream &write(std::ofstream &stream);

  /**
   * @brief Compile N bytes taken from an iterator into one unsigned integer in the big endian order
   * @param[in] iter Iterator pointing to the first byte
   * @param[in] end Iterator pointing to the end of the container
   */
  template <typename Iter, size_t N = 4>
  static uint32_t compileValues(Iter iter, Iter end);

  /**
   * @brief Declare the DB table fields of the telemetry
   * @param[in,out] sink Sink which receives the field definitions
   * @param[in] table_name Name of the DB table
   */
  void initializeDBTable(DBFieldSink *sink, const std::string &table_name) const override;
  /**
   * @brief Send the current values of the telemetry to the DB sink
   * @param[in,out] sink Sink which receives the values
   */
  void serialize(DBFieldSink *sink) const override;
};

template <typename Iter, size_t N>
uint32_t BaseTelemetryDefinition::compileValues(Iter iter, Iter end) {
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
} // namespace gramsballoon::pgrams
#endif // GRAMSBalloon_BaseTelemetryDefinition_hh