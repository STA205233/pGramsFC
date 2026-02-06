#include "MySQLManager.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus MySQLManager::mod_define() {
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
  define_parameter("check_exist", &mod_class::checkExist_);
  set_parameter_description("Check existence of tables and columns before inserting data");
  return AS_OK;
}
ANLStatus MySQLManager::mod_initialize() {
  mysqlIO_.SetCheckExist(checkExist_);
  mysqlIO_.Initialize(host_, port_, user_, password_, database_);
  if (!mysqlIO_.connected()) {
    std::cerr << module_name() << "::mod_initialize: Failed to connect to MySQL server." << std::endl;
    return AS_OK;
  }
  return AS_OK;
}

ANLStatus MySQLManager::mod_finalize() {
  return AS_OK;
}

} // namespace gramsballoon::pgrams