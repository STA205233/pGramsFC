#include "TreatToFCallback.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus TreatToFCallback::mod_define() {
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
  define_parameter("InterpretTelemetry_name", &mod_class::interpretTelemetryName_);
  define_parameter("check_exist", &mod_class::checkExist_);
  set_parameter_description("Check existence of tables and columns before inserting data");
  define_parameter("table_name", &mod_class::tableName_);
  set_parameter_description("Table name for HubHK telemetry data");
  return AS_OK;
}
ANLStatus TreatToFCallback::mod_initialize() {
  if (exist_module(interpretTelemetryName_)) {
    get_module(interpretTelemetryName_, &interpretTelemetry_);
  }
  else {
    std::cerr << module_id() << ": " << interpretTelemetryName_ << "doesn't exists" << std::endl;
    return AS_QUIT_ALL_ERROR;
  }
  mysqlIO_.SetCheckExist(checkExist_);
  mysqlIO_.Initialize(host_, port_, user_, password_, database_);
  if (!mysqlIO_.connected()) {
    std::cerr << module_id() << "::mod_initialize: Failed to connect to MySQL server." << std::endl;
    return AS_OK;
  }
  mysqlFieldSink_.setMySQLIO(&mysqlIO_);
  interpretTelemetry_->initializeDBTableInSink(&mysqlFieldSink_, tableName_);
  mysqlFieldSink_.addField("status", static_cast<uint32_t>(0));
  mysqlFieldSink_.addField("status_name", "UNKNOWN");
  if (mysqlIO_.CheckTableExistence(tableName_)) {
    if (chatter_ > 0) {
      std::cout << module_id() << "::mod_initialize: Table (" << tableName_ << ") already exists." << std::endl;
    }
  }
  else {
    if (chatter_ > 0) {
      std::cout << module_id() << "::mod_initialize: Table (" << tableName_ << ") does not exist. Create the table." << std::endl;
    }
    mysqlIO_.CreateTable(tableName_);
  }
  mysqlIO_.PrintTableInfo(tableName_);
  return AS_OK;
}
ANLStatus TreatToFCallback::mod_analyze() {
  if (!interpretTelemetry_) {
    std::cerr << module_id() << ": " << interpretTelemetryName_ << "is nullptr" << std::endl;
    return AS_ERROR;
  }
  if (interpretTelemetry_->CurrentTelemetryType() != 2) {
    return AS_OK;
  }
  const auto telemetry = interpretTelemetry_->getTelemetry();
  const auto contents = telemetry->getContents();
  if (contents->Argc() != 2) {
    std::cerr << module_id() << ": The number of argument is incorrect: " << contents->Argc() << std::endl;
    return AS_ERROR;
  }
  const auto args = contents->Arguments();
  telemetry->serialize(&mysqlFieldSink_);
  command_ = args[0];
  statusCode_ = args[1];
  mysqlFieldSink_.setFieldValue("status", statusCode_);
  if (statusCode_ == static_cast<uint32_t>(TOF_STATUS::SUCCESS)) {
    status_ = TOF_STATUS::SUCCESS;
    mysqlFieldSink_.setFieldValue("status_name", "Success");
  }
  else if (statusCode_ == static_cast<uint32_t>(TOF_STATUS::FAILED)) {
    status_ = TOF_STATUS::FAILED;
    mysqlFieldSink_.setFieldValue("status_name", "Failed");
  }
  else if (statusCode_ == static_cast<uint32_t>(TOF_STATUS::INTERRUPTED)) {
    status_ = TOF_STATUS::INTERRUPTED;
    mysqlFieldSink_.setFieldValue("status_name", "Interrupted");
  }
  else {
    status_ = TOF_STATUS::UNKNOWN;
    mysqlFieldSink_.setFieldValue("status_name", "Unknown");
  }
  mysqlIO_.Insert(tableName_);
  return AS_OK;
}
} // namespace gramsballoon::pgrams