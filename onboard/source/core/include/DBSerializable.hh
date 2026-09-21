#ifndef GB_DBSerializable_HH
#define GB_DBSerializable_HH 1
#include <string>
namespace gramsballoon::pgrams {
class DBFieldSink;

/**
  @brief Virtual class to push database
  @author Shota Arai
  @date 2026-06-04 | Shota Arai | Detached from BaseTelemetryDefinition
  @ingroup ground_core
 */
class DBSerializable {
  /**
   * @brief Initialize DB table structure (setting column names and types...)
   * @param[in,out] sink DBFieldSink object
   * @param[in] table_name Name of the table which receives the data
   */
  virtual void initializeDBTable(DBFieldSink *sink, const std::string &table_name) const = 0;
  /**
   * @brief Send the current values of the data to the DB field sink
   * @param[in,out] sink DBFieldSink object
   */
  virtual void serialize(DBFieldSink *sink) const = 0;
};
} // namespace gramsballoon::pgrams

#endif //GB_DBSerializable_HH