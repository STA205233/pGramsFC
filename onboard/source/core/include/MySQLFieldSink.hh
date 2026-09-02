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
 */
class MySQLFieldSink: public DBFieldSink {
public:
  MySQLFieldSink() = default;
  virtual ~MySQLFieldSink() = default;
  void setMySQLIO(mysql::MySQLIO *mysqlIO) {
    mysqlIO_ = mysqlIO;
  }

  std::string getTableName() const {
    return tableName_;
  }

  void initializeTable(const std::string &table_name) override {
    if (!mysqlIO_) {
      std::cerr << "MySQLIO is not set in MySQLFieldSink" << std::endl;
      return;
    }
    tableName_ = table_name;
    mysqlIO_->AddTable(table_name);
  }
  void addField(const std::string &field_name, double) override {
    addFieldImpl<double>(field_name);
  }
  void addField(const std::string &field_name, float) override {
    addFieldImpl<float>(field_name);
  }
  void addField(const std::string &field_name, uint64_t) override {
    addFieldImpl<uint64_t>(field_name);
  }
  void addField(const std::string &field_name, int64_t) override {
    addFieldImpl<int64_t>(field_name);
  }
  void addField(const std::string &field_name, uint32_t) override {
    addFieldImpl<uint32_t>(field_name);
  }
  void addField(const std::string &field_name, int32_t) override {
    addFieldImpl<int32_t>(field_name);
  }
  void addField(const std::string &field_name, uint16_t) override {
    addFieldImpl<uint16_t>(field_name);
  }
  void addField(const std::string &field_name, int16_t) override {
    addFieldImpl<int16_t>(field_name);
  }
  void addField(const std::string &field_name, uint8_t) override {
    addFieldImpl<uint8_t>(field_name);
  }
  void addField(const std::string &field_name, const std::string &) override {
    addFieldImpl<std::string>(field_name);
  }
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
  template <typename T>
  void fieldImpl(const std::string &field_name, const T &value) {
    if (!mysqlIO_) {
      std::cerr << "MySQLIO is not set in MySQLFieldSink" << std::endl;
      return;
    }
    mysqlIO_->SetItem(tableName_, field_name, mysqlx::Value(value));
  }
  virtual void setFieldValue(const std::string &field_name, const uint8_t &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const int16_t &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const uint16_t &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const int32_t &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const uint32_t &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const int64_t &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const uint64_t &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const float &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const double &value) override {
    fieldImpl(field_name, value);
  }
  virtual void setFieldValue(const std::string &field_name, const std::string &value) override {
    fieldImpl(field_name, value);
  }

private:
  mysql::MySQLIO *mysqlIO_ = nullptr;
  std::string tableName_ = "";
};
} // namespace gramsballoon::pgrams
#endif //GB_MySQLFieldSink_hh