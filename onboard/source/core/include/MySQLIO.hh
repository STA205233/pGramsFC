#ifndef GRAMSBalloon_MySQLIO_hh
#define GRAMSBalloon_MySQLIO_hh 1
#include "DBFieldSink.hh"
#include "mysqlx/xdevapi.h"
#include <boost/mp11.hpp>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#define TRY_AND_CATCH_MYSQL_EXCEPTIONS_BEGIN \
  try {
#define TRY_AND_CATCH_MYSQL_EXCEPTIONS_END(process) \
  }                                                 \
  catch (const mysqlx::Error &e) {                  \
    std::cerr << "Error: " << e << std::endl;       \
    process;                                        \
  }

namespace gramsballoon::pgrams::mysql {
template <class T>
struct SqlType;
template <>
struct SqlType<uint8_t> {
  static std::string name() {
    return "TINYINT UNSIGNED";
  }
};
template <>
struct SqlType<int16_t> {
  static std::string name() {
    return "SMALLINT";
  }
};
template <>
struct SqlType<uint16_t> {
  static std::string name() {
    return "SMALLINT UNSIGNED";
  }
};
template <>
struct SqlType<int32_t> {
  static std::string name() {
    return "INT";
  }
};
template <>
struct SqlType<uint32_t> {
  static std::string name() {
    return "INT UNSIGNED";
  }
};
template <>
struct SqlType<int64_t> {
  static std::string name() {
    return "BIGINT";
  }
};
template <>
struct SqlType<uint64_t> {
  static std::string name() {
    return "BIGINT UNSIGNED";
  }
};
template <>
struct SqlType<float> {
  static std::string name() {
    return "FLOAT";
  }
};
template <>
struct SqlType<double> {
  static std::string name() {
    return "DOUBLE";
  }
};
template <size_t N>
struct VarChar {
  static constexpr size_t n = N;
};
template <size_t N>
struct SqlType<VarChar<N>> {
  static std::string name() {
    return "VARCHAR(" + std::to_string(N) + ")";
  }
};
template <>
struct SqlType<std::string> {
  static std::string name() {
    return SqlType<VarChar<255>>::name();
  }
};

using value_t = mysqlx::Value;
using table_t = std::map<std::string, std::pair<std::optional<value_t>, std::string>>; // column_name -> (value, type)
/**
 * @brief A class for MySQL I/O operations using MySQL X DevAPI.
 * @author Shota Arai
 * @date 2025-02-** | First design
 * @date 2025-12-14 | Shota Arai | Refactoring to use a MySQLFieldSink class (v2.0)
 */
class MySQLIO {

public:
  MySQLIO() = default;
  virtual ~MySQLIO() = default;

private:
  std::map<std::string, table_t> tables_;
  std::shared_ptr<mysqlx::Session> session_;
  std::optional<mysqlx::Schema> schema_ = std::nullopt;
  bool checkExist_ = true;
  bool connected_ = false;

public:
  void Initialize(const std::string &host, const int port, const std::string &user, const std::string &password, const std::string &database);
  void AddTable(const std::string &table_name);
  bool CheckTableExistence(const std::string &table_name) {
    if (!schema_) {
      std::cerr << "Schema is not initialized" << std::endl;
      return false;
    }
    return schema_->getTable(table_name, false).existsInDatabase();
  }
  void CreateTable(const std::string &table_name);
  template <typename T>
  void AddColumn(const std::string &table_name, const std::string &col_name) {
    const auto it = tables_.find(table_name);
    if (it == tables_.end()) {
      std::cerr << "Table (" << table_name << ") must be resisgered to MySQLIO before adding column." << std::endl;
      return;
    }
    const auto it2 = it->second.find(col_name);
    if (it2 != it->second.end()) {
      std::cerr << col_name << "is already resisgered in Table(" << table_name << ")" << std::endl;
      return;
    }
    it->second.insert(std::make_pair(col_name, std::make_pair(std::nullopt, SqlType<T>::name())));
  }
  void SetItem(const std::string &table_name, const std::string &col_name, const mysqlx::Value &value);
  bool hasKeyInTable(const std::string &table_name, const std::string &col_name) {
    const auto it = tables_.find(table_name);
    if (it == tables_.end()) {
      return false;
    }
    const auto it2 = it->second.find(col_name);
    return it2 != it->second.end();
  }
  void Insert(const std::string &table_name);
  void SetCheckExist(const bool checkExist) { checkExist_ = checkExist; }
  void PrintTableInfo(const std::string &table_name);
  bool connected() const { return connected_; }
};

} // namespace gramsballoon::pgrams::mysql
#endif //GRAMSBalloon_MySQLIO_hh