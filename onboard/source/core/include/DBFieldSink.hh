#ifndef GRAMSBalloon_DBFieldSink_hh
#define GRAMSBalloon_DBFieldSink_hh 1
#include <cstdint>
#include <string>
namespace gramsballoon::pgrams {
/**
 * @brief A tag type which lets addField() create a text field. Text type expresses text with variable length.
 */
class Text {};
/**
 * @brief Interface class for database field sink
 * @author Shota Arai
 * @date 2025-12-14 | Shota Arai | Created
 */
class DBFieldSink {
public:
  DBFieldSink() = default;
  virtual ~DBFieldSink() = default;
  /**
   * @brief Prepare the table which receives the fields
   * @param[in] table_name Name of the table
   */
  virtual void initializeTable(const std::string & /*table_name*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const uint8_t & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const int16_t & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const uint16_t & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const int32_t & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const uint32_t & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const int64_t & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const uint64_t & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const float & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const double & /*dummy*/) {}
  /**
   * @brief Set the value of a field of the current record
   * @param[in] field_name Name of the field
   * @param[in] dummy Value to be set
   */
  virtual void setFieldValue(const std::string & /*field_name*/, const std::string & /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, uint8_t /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, int16_t /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, uint16_t /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, int32_t /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, uint32_t /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, int64_t /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, uint64_t /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, float /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, double /*dummy*/) {}
  /**
   * @brief Add a field to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, const std::string & /*dummy*/) {}
  /**
   * @brief Add a text field to the table
   * @param[in] field_name Name of the field
   * @param[in] dummy Dummy argument which specifies the type of the field
   */
  virtual void addField(const std::string & /*field_name*/, const Text & /*dummy*/) {}
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_DBFieldSink_hh