#include "PushToMySQL.hh"
#include "HubHKTelemetry.hh"
#include <algorithm>
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus PushToMySQL::mod_define() {
  define_parameter("chatter", &mod_class::chatter_);
  define_parameter("host", &mod_class::host_);
  set_parameter_description("MySQL server host");
  define_parameter("port", &mod_class::port_);
  set_parameter_description("MySQL server port");
  define_parameter("user", &mod_class::user_);
  set_parameter_description("MySQL user name");
  define_parameter("password", &mod_class::password_);
  set_parameter_description("MySQL user password");
  define_parameter("database", &mod_class::database_);
  set_parameter_description("MySQL database name");
  define_parameter("HubHKInterpreter_name", &mod_class::HubHKInterpreterName_);
  set_parameter_description("Module name of InterpretTelemetry for HubHK");
  define_parameter("HubHK_table_name", &mod_class::HubHKtableName_);
  set_parameter_description("Table name for HubHK telemetry data");
  define_parameter("check_exist", &mod_class::checkExist_);
  set_parameter_description("Check existence of tables and columns before inserting data");
  return AS_OK;
}
ANLStatus PushToMySQL::mod_initialize() {
  if (exist_module(HubHKInterpreterName_)) {
    get_module_NC(HubHKInterpreterName_, &HubHKinterpreter_);
  }
  mysqlIO_.SetCheckExist(checkExist_);
  mysqlIO_.Initialize(host_, port_, user_, password_, database_);
  if (!mysqlIO_.connected()) {
    std::cerr << module_name() << "::mod_initialize: Failed to connect to MySQL server." << std::endl;
    return AS_OK;
  }
  mysqlFieldSink_.setMySQLIO(&mysqlIO_);
  HubHKinterpreter_->initializeDBTableInSink(&mysqlFieldSink_, HubHKtableName_);
  if (mysqlIO_.CheckTableExistence(HubHKtableName_)) {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << HubHKtableName_ << ") already exists." << std::endl;
    }
  }
  else {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << HubHKtableName_ << ") does not exist. Create the table." << std::endl;
    }
    mysqlIO_.CreateTable(HubHKtableName_);
  }
  mysqlIO_.PrintTableInfo(HubHKtableName_);
  return AS_OK;
}
ANLStatus PushToMySQL::mod_analyze() {
  if (!mysqlIO_.connected()) {
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
  mysqlIO_.Insert(HubHKtableName_);
  return AS_OK;
}
ANLStatus PushToMySQL::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams