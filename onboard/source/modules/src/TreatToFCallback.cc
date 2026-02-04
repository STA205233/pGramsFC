#include "TreatToFCallback.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus TreatToFCallback::mod_define() {
  define_parameter("chatter", &mod_class::chatter_);
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
  if (exist_module("MySQLManager")) {
    get_module_NC("MySQLManager", &mysqlManager_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize MySQLManager module is not found." << std::endl;
    return AS_QUIT_ERROR;
  }
  auto mysqlIO = mysqlManager_->getMySQLIO();
  mysqlFieldSink_.setMySQLIO(mysqlIO);
  interpretTelemetry_->initializeDBTableInSink(&mysqlFieldSink_, tableName_);
  mysqlFieldSink_.addField("command", static_cast<uint16_t>(0));
  mysqlFieldSink_.addField("status", static_cast<uint32_t>(0));
  mysqlFieldSink_.addField("status_name", "UNKNOWN");
  if (mysqlIO->CheckTableExistence(tableName_)) {
    if (chatter_ > 0) {
      std::cout << module_id() << "::mod_initialize: Table (" << tableName_ << ") already exists." << std::endl;
    }
  }
  else {
    if (chatter_ > 0) {
      std::cout << module_id() << "::mod_initialize: Table (" << tableName_ << ") does not exist. Create the table." << std::endl;
    }
    mysqlIO->CreateTable(tableName_);
  }
  mysqlIO->PrintTableInfo(tableName_);
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
  auto mysqlIO = mysqlManager_->getMySQLIO();
  if (!mysqlIO->connected()) {
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
  mysqlFieldSink_.setFieldValue("command", command_);
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
  mysqlIO->Insert(tableName_);
  return AS_OK;
}
} // namespace gramsballoon::pgrams