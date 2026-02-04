#include "PushToMySQL.hh"
#include "HubHKTelemetry.hh"
#include <algorithm>
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus PushToMySQL::mod_define() {
  define_parameter("chatter", &mod_class::chatter_);
  define_parameter("HubHKInterpreter_name", &mod_class::HubHKInterpreterName_);
  set_parameter_description("Module name of InterpretTelemetry for HubHK");
  define_parameter("HubHK_table_name", &mod_class::HubHKtableName_);
  set_parameter_description("Table name for HubHK telemetry data");
  return AS_OK;
}
ANLStatus PushToMySQL::mod_initialize() {
  if (exist_module(HubHKInterpreterName_)) {
    get_module_NC(HubHKInterpreterName_, &HubHKinterpreter_);
  }
  if (exist_module("MySQLManager")) {
    get_module_NC("MySQLManager", &mysqlManager_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize MySQLManager module is not found." << std::endl;
    return AS_QUIT_ERROR;
  }
  auto mysqlIO = mysqlManager_->getMySQLIO();
  mysqlFieldSink_.setMySQLIO(mysqlIO);
  HubHKinterpreter_->initializeDBTableInSink(&mysqlFieldSink_, HubHKtableName_);
  if (mysqlIO->CheckTableExistence(HubHKtableName_)) {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << HubHKtableName_ << ") already exists." << std::endl;
    }
  }
  else {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << HubHKtableName_ << ") does not exist. Create the table." << std::endl;
    }
    mysqlIO->CreateTable(HubHKtableName_);
  }
  mysqlIO->PrintTableInfo(HubHKtableName_);
  return AS_OK;
}
ANLStatus PushToMySQL::mod_analyze() {
  auto mysqlIO = mysqlManager_->getMySQLIO();
  if (!mysqlIO->connected()) {
    return AS_OK;
  }
  if (!HubHKinterpreter_) {
    std::cerr << module_name() << "::mod_analyze: HubHKinterpreter is not set." << std::endl;
    return AS_OK;
  }
  if (HubHKinterpreter_->CurrentTelemetryType() == 0) { // no received telemetry
    return AS_OK;
  }
  HubHKinterpreter_->pushToDBSink(&mysqlFieldSink_);
  mysqlIO->Insert(HubHKtableName_);
  return AS_OK;
}
ANLStatus PushToMySQL::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams