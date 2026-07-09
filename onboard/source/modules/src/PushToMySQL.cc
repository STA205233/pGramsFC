#include "PushToMySQL.hh"
#include "HubHKTelemetry.hh"
#include <algorithm>
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus PushToMySQL::mod_define() {
  define_parameter("chatter", &mod_class::chatter_);
  define_parameter("data_store_name", &mod_class::dataStoreName_);
  set_parameter_description("Module name of data store module");
  define_parameter("table_name", &mod_class::tableName_);
  set_parameter_description("Table name for data");
  return AS_OK;
}
ANLStatus PushToMySQL::mod_initialize() {
  const anlnext::BasicModule *module_ptr = nullptr;
  if (exist_module(dataStoreName_)) {
    get_module(dataStoreName_, &module_ptr);
    dataStore_ = dynamic_cast<const VDBDataStore *>(module_ptr);
    if (!dataStore_) {
      std::cerr << module_id() << "::mod_initialize: paramter \"data_store_name\" is not a class inherited from VDBDataStore" << std::endl;
      return AS_QUIT_ALL_ERROR;
    }
  }
  else {
    std::cerr << module_id() << "::mod_initialize: module " << dataStoreName_ << " not found" << std::endl;
    return AS_QUIT_ALL_ERROR;
  }
  if (exist_module("MySQLManager")) {
    get_module_NC("MySQLManager", &mysqlManager_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize MySQLManager module is not found." << std::endl;
    return AS_QUIT_ALL_ERROR;
  }
  auto mysqlIO = mysqlManager_->getMySQLIO();
  mysqlFieldSink_.setMySQLIO(mysqlIO);
  dataStore_->initializeDBTableInSink(&mysqlFieldSink_, tableName_);
  if (mysqlIO->CheckTableExistence(tableName_)) {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << tableName_ << ") already exists." << std::endl;
    }
  }
  else {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << tableName_ << ") does not exist. Create the table." << std::endl;
    }
    mysqlIO->CreateTable(tableName_);
  }
  mysqlIO->PrintTableInfo(tableName_);
  return AS_OK;
}
ANLStatus PushToMySQL::mod_analyze() {
  auto mysqlIO = mysqlManager_->getMySQLIO();
  if (!mysqlIO->connected()) {
    return AS_OK;
  }
  if (!dataStore_) {
    std::cerr << module_name() << "::mod_analyze: HubHKinterpreter is not set." << std::endl;
    return AS_OK;
  }
  if (!dataStore_->hasData()) { // no received telemetry
    return AS_OK;
  }
  dataStore_->pushToDBSink(&mysqlFieldSink_);
  mysqlIO->Insert(tableName_);
  return AS_OK;
}
ANLStatus PushToMySQL::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams