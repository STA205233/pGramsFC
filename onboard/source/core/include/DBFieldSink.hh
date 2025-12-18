#ifndef GRAMSBalloon_DBFieldSink_hh
#define GRAMSBalloon_DBFieldSink_hh 1
#include <string>
#include <vector>
namespace gramsballoon::pgrams {

/**
 * @brief Interface class for database field sink
 * @author Shota Arai
 * @date 2025-12-14 | Shota Arai | Created
 */
class DBFieldSink {
public:
  DBFieldSink() = default;
  virtual ~DBFieldSink() = default;
  virtual void initializeTable(const std::string & /*table_name*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const uint8_t & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const int16_t & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const uint16_t & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const int32_t & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const uint32_t & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const int64_t & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const uint64_t & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const float & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const double & /*value*/) {}
  virtual void setFieldValue(const std::string & /*field_name*/, const std::string & /*value*/) {}
  virtual void addField(const std::string & /*field_name*/, uint8_t /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, int16_t /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, uint16_t /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, int32_t /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, uint32_t /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, int64_t /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, uint64_t /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, float /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, double /*dummy*/) {}
  virtual void addField(const std::string & /*field_name*/, const std::string & /*dummy*/) {}
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_DBFieldSink_hh