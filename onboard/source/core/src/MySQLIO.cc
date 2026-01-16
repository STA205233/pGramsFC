#include "MySQLIO.hh"
#include <iostream>
namespace gramsballoon::pgrams::mysql {

void MySQLIO::AddTable(const std::string &table_name) {
  if (!schema_) {
    std::cerr << "Schema is not initialized" << std::endl;
    return;
  }
  const auto it = tables_.find(table_name);
  if (it != tables_.end()) {
    std::cerr << "Table (" << table_name << ") is already resistered to MySQLIO" << std::endl;
    return;
  }
  tables_[table_name] = table_t();
  std::cout << "Table (" << table_name << ") is resistered to MySQLIO" << std::endl;
}
void MySQLIO::Insert(const std::string &table_name) {
  if (!schema_) {
    std::cerr << "Schema is not initialized" << std::endl;
    return;
  }
  TRY_AND_CATCH_MYSQL_EXCEPTIONS_BEGIN;
  std::vector<mysqlx::Value> cols;
  std::vector<std::string> cols_name;
  for (auto &col: tables_[table_name]) {
    if (!col.second.first.has_value()) {
      continue;
    }
    else {
      cols_name.push_back(col.first);
      cols.push_back(col.second.first.value());
    }
  }
  auto table_insert = schema_->getTable(table_name, checkExist_).insert(cols_name);
  table_insert.values(cols);
  table_insert.execute();
  TRY_AND_CATCH_MYSQL_EXCEPTIONS_END();
  for (auto &col: tables_[table_name]) {
    col.second.first.reset();
  }
}
void MySQLIO::Initialize(const std::string &host, const int port, const std::string &user, const std::string &password, const std::string &database) {
  TRY_AND_CATCH_MYSQL_EXCEPTIONS_BEGIN;
  session_ = std::make_shared<mysqlx::Session>(host, port, user, password);
  schema_ = session_->getSchema(database, checkExist_);
  if (!schema_->existsInDatabase()) {
    std::cout << "Database (" << database << ") does not exist. Create the database." << std::endl;
    session_->createSchema(database);
    schema_ = session_->getSchema(database);
  }
  connected_ = true;
  TRY_AND_CATCH_MYSQL_EXCEPTIONS_END(connected_ = false);
}

void MySQLIO::SetItem(const std::string &table_name, const std::string &col_name, const mysqlx::Value &value) {
  const auto it = tables_.find(table_name);
  if (it == tables_.end()) {
    std::cerr << "Table (" << table_name << ") is not resistered to MySQLIO" << std::endl;
    return;
  }
  const auto it2 = it->second.find(col_name);
  if (it2 == it->second.end()) {
    std::cerr << "Table (" << table_name << ") doesn't have column (" << col_name << ")." << std::endl;
    return;
  }
  it2->second.first = value;
}
void MySQLIO::PrintTableInfo(const std::string &table_name) {
  std::cout << "Table: " << table_name << std::endl;
  for (auto &col: tables_[table_name]) {
    std::cout << col.first << " ";
  }
  std::cout << std::endl;
}
void MySQLIO::CreateTable(const std::string &table_name) {
  if (!schema_) {
    std::cerr << "Schema is not initialized" << std::endl;
    return;
  }
  TRY_AND_CATCH_MYSQL_EXCEPTIONS_BEGIN;
  if (schema_->getTable(table_name, false).existsInDatabase()) {
    std::cout << "Table (" << table_name << ") is already exists in database." << std::endl;
    return;
  }
  TRY_AND_CATCH_MYSQL_EXCEPTIONS_END();
  std::stringstream create_table_query;
  TRY_AND_CATCH_MYSQL_EXCEPTIONS_BEGIN;
  create_table_query << "CREATE TABLE IF NOT EXISTS " << schema_->getName() << "." << table_name << " (";
  create_table_query << "id INT AUTO_INCREMENT PRIMARY KEY, receive_time DATETIME DEFAULT CURRENT_TIMESTAMP, ";
  for (auto it = tables_[table_name].begin(); it != tables_[table_name].end(); it++) {
    create_table_query << it->first << " " << it->second.second;
    if (it != (--tables_[table_name].end())) {
      create_table_query << ", ";
    }
  }
  create_table_query << ");";
  const auto result = session_->sql(create_table_query.str()).execute();
  std::cout << "Table (" << table_name << ") is created in database." << std::endl;
  TRY_AND_CATCH_MYSQL_EXCEPTIONS_END(std::cerr << "Query: " << create_table_query.str() << std::endl;);
}
} // namespace gramsballoon::pgrams::mysql