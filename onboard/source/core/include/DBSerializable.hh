#ifndef GB_DBSerializable_HH
#define GB_DBSerializable_HH 1
#include <string>
namespace gramsballoon::pgrams {
class DBFieldSink;

/**
  @brief Virtual class to push database
  @author Shota Arai
  @date 2026-06-04 | Shota Arai | Detached from BaseTelemetryDefinition
 */
class DBSerializable {
  /**
   * @brief Initialize DB table structure (setting column names and types...)
   * @param sink DBFieldSink object
   */
  virtual void initializeDBTable(DBFieldSink *sink, const std::string &table_name) const = 0;
  virtual void serialize(DBFieldSink *sink) const = 0;
};
} // namespace gramsballoon::pgrams

#endif //GB_DBSerializable_HH