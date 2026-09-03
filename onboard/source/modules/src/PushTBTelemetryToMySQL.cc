#include "PushTBTelemetryToMySQL.hh"
#include "CommunicationCodes.hh"
#include "InterpretTelemetry.hh"
#include "MySQLFieldSink.hh"
#include "MySQLManager.hh"
#include "ToFBiasTelemetry.hh"
#include <iostream>
#include <memory>
#include <string>

using namespace ::pgrams::communication;
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus PushTBTelemetryToMySQL::mod_define() {
  define_parameter("InterpretTelemetry_name", &mod_class::interpretTelemetryName_);
  set_parameter_description("Module name of data store module");
  define_parameter("table_name_full", &mod_class::tableNameForFullOutput_);
  set_parameter_description("Table name for full output");
  define_parameter("table_name_summary", &mod_class::tableNameForSummaryOutput_);
  set_parameter_description("Table name for summary output");
  define_parameter("chatter", &mod_class::chatter_);
  return AS_OK;
}
ANLStatus PushTBTelemetryToMySQL::mod_initialize() {
  if (exist_module(interpretTelemetryName_)) {
    get_module_IF(interpretTelemetryName_, &interpretTelemetry_);
    if (!interpretTelemetry_) {
      std::cerr << module_id() << "::mod_initialize: paramter \"data_store_name\" is not a class inherited from VDBDataStore" << std::endl;
      return AS_QUIT_ALL_ERROR;
    }
  }
  else {
    std::cerr << module_id() << "::mod_initialize: module " << interpretTelemetryName_ << " not found" << std::endl;
    return AS_QUIT_ALL_ERROR;
  }
  if (exist_module("MySQLManager")) {
    get_module_NC("MySQLManager", &mysqlManager_);
  }
  else {
    std::cerr << module_id() << "::mod_initialize MySQLManager module is not found." << std::endl;
    return AS_QUIT_ALL_ERROR;
  }

  mysqlFieldSinkForFull_ = std::make_shared<MySQLFieldSink>();
  mysqlFieldSinkForSummary_ = std::make_shared<MySQLFieldSink>();

  auto mysqlIO = mysqlManager_->getMySQLIO();

  return AS_OK;
}
void PushTBTelemetryToMySQL::initializeMysqlField(mysql::MySQLIO *mysql_io) {
  mysqlFieldSinkForFull_->setMySQLIO(mysql_io);
  const ToFBiasTelemetry *telemetry = dynamic_cast<const ToFBiasTelemetry *>(interpretTelemetry_->getTelemetry());
  telemetry->initializeDBTableFull(mysqlFieldSinkForFull_.get(), tableNameForFullOutput_);
  if (mysql_io->CheckTableExistence(tableNameForFullOutput_)) {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << tableNameForFullOutput_ << ") already exists." << std::endl;
    }
  }
  else {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << tableNameForFullOutput_ << ") does not exist. Create the table." << std::endl;
    }
    mysql_io->CreateTable(tableNameForFullOutput_);
  }
  mysql_io->PrintTableInfo(tableNameForFullOutput_);

  // summary
  mysqlFieldSinkForSummary_->setMySQLIO(mysql_io);
  telemetry->initializeDBTableSummary(mysqlFieldSinkForSummary_.get(), tableNameForSummaryOutput_);
  if (mysql_io->CheckTableExistence(tableNameForSummaryOutput_)) {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << tableNameForSummaryOutput_ << ") already exists." << std::endl;
    }
  }
  else {
    if (chatter_ > 0) {
      std::cout << module_name() << "::mod_initialize: Table (" << tableNameForSummaryOutput_ << ") does not exist. Create the table." << std::endl;
    }
    mysql_io->CreateTable(tableNameForSummaryOutput_);
  }

  mysql_io->PrintTableInfo(tableNameForSummaryOutput_);
}

ANLStatus PushTBTelemetryToMySQL::mod_analyze() {
  auto mysqlIO = mysqlManager_->getMySQLIO();
  if (!mysqlIO->connected()) {
    return AS_OK;
  }
  if (!interpretTelemetry_) {
    std::cerr << module_name() << "::mod_analyze: " << interpretTelemetryName_ << " is not set." << std::endl;
    return AS_OK;
  }
  switch (interpretTelemetry_->CurrentTelemetryType()) {
  case to_telem_u16(TelemetryCodes::HUB_Tof_Bias_full):
    interpretTelemetry_->pushToDBSink(mysqlFieldSinkForFull_.get());
    mysqlIO->Insert(tableNameForFullOutput_);
    break;
  case to_telem_u16(TelemetryCodes::HUB_Tof_Bias_summary):
    interpretTelemetry_->pushToDBSink(mysqlFieldSinkForSummary_.get());
    mysqlIO->Insert(tableNameForSummaryOutput_);
    break;
  default:
    return AS_OK;
  }
  return AS_OK;
}
ANLStatus PushTBTelemetryToMySQL::mod_finalize() {
  return AS_OK;
}
} // namespace gramsballoon::pgrams