#ifndef GB_MySQLFieldSink_hh
#define GB_MySQLFieldSink_hh 1
#include "DBFieldSink.hh"
#include "MySQLIO.hh"
#include <cstdint>
#include <iostream>
#include <string>
namespace gramsballoon::pgrams {

/**
 * @brief A DBFieldSink implementation for MySQL database using MySQLIO.
 * @author Shota Arai
 * @date 2025-12-14 | First design
 * @ingroup ground_core
 */
class MySQLFieldSink: public DBFieldSink {
public:
  MySQLFieldSink() = default;
  virtual ~MySQLFieldSink() = default;
  /**
   * @brief Set the MySQLIO object which accesses the database
   * @param[in] mysqlIO Pointer to the MySQLIO object
   */
  void setMySQLIO(mysql::MySQLIO *mysqlIO) {
    mysqlIO_ = mysqlIO;
  }

  /**
   * @brief Return the name of the table currently used
   * @return Name of the table
   */
  std::string getTableName() const {
    return tableName_;
  }

  /**
   * @brief Add the table to the database and use it as the destination of the following calls
   * @param[in] table_name Name of the table
   */
  void initializeTable(const std::string &table_name) override {
    if (!mysqlIO_) {
      std::cerr << "MySQLIO is not set in MySQLFieldSink" << std::endl;
      return;
    }
    tableName_ = table_name;
    mysqlIO_->AddTable(table_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, double /*dummy*/) override {
    addFieldImpl<double>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, float /*dummy*/) override {
    addFieldImpl<float>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, uint64_t /*dummy*/) override {
    addFieldImpl<uint64_t>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, int64_t /*dummy*/) override {
    addFieldImpl<int64_t>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, uint32_t /*dummy*/) override {
    addFieldImpl<uint32_t>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, int32_t /*dummy*/) override {
    addFieldImpl<int32_t>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, uint16_t /*dummy*/) override {
    addFieldImpl<uint16_t>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, int16_t /*dummy*/) override {
    addFieldImpl<int16_t>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, uint8_t /*dummy*/) override {
    addFieldImpl<uint8_t>(field_name);
  }
  /**
   * @brief Add a column to the table, whose type is given by the type of dummy
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, const std::string & /*dummy*/) override {
    addFieldImpl<std::string>(field_name);
  }
  /**
   * @brief Add a text column to the table
   * @param[in] field_name Name of the column
   * @param[in] dummy Dummy argument which specifies the type of the column
   */
  void addField(const std::string &field_name, const Text & /*dummy*/) override {
    addFieldImpl<Text>(field_name);
  }
  /**
   * @brief Add a column of the type T to the table unless it already exists
   * @param[in] field_name Name of the column
   */
  template <typename T>
  void addFieldImpl(const std::string &field_name) {
    if (!mysqlIO_) {
      std::cerr << "MySQLIO is not set in MySQLFieldSink" << std::endl;
      return;
    }
    if (!mysqlIO_->hasKeyInTable(tableName_, field_name)) {
      mysqlIO_->AddColumn<T>(tableName_, field_name);
    }
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  template <typename T>
  void fieldImpl(const std::string &field_name, const T &value) {
    if (!mysqlIO_) {
      std::cerr << "MySQLIO is not set in MySQLFieldSink" << std::endl;
      return;
    }
    mysqlIO_->SetItem(tableName_, field_name, mysqlx::Value(value));
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const uint8_t &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const int16_t &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const uint16_t &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const int32_t &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const uint32_t &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const int64_t &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const uint64_t &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const float &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const double &value) override {
    fieldImpl(field_name, value);
  }
  /**
   * @brief Set the value of a column of the current record
   * @param[in] field_name Name of the column
   * @param[in] value Value to be set
   */
  virtual void setFieldValue(const std::string &field_name, const std::string &value) override {
    fieldImpl(field_name, value);
  }

private:
  mysql::MySQLIO *mysqlIO_ = nullptr;
  std::string tableName_ = "";
};
} // namespace gramsballoon::pgrams
#endif //GB_MySQLFieldSink_hh