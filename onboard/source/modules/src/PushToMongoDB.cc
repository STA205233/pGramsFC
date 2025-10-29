#include "PushToMongoDB.hh"

#include "ErrorManager.hh"
#include <boost/format.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <cstdint>
#include <hsquicklook/DocumentBuilder.hh>

using namespace anlnext;

namespace gramsballoon {

PushToMongoDB::PushToMongoDB() {
}

ANLStatus PushToMongoDB::mod_define() {
  define_parameter("chatter", &mod_class::chatter_);

  return AS_OK;
}

ANLStatus PushToMongoDB::mod_initialize() {
  get_module_NC("MongoDBClient", &mongodbClient_);
  mongodbClient_->createCappedCollection("grams", 1 * 1024 * 1024);

  get_module_NC("InterpretTelemetry", &interpreter_);

  return AS_OK;
}

ANLStatus PushToMongoDB::mod_analyze() {
  if (interpreter_->CurrentTelemetryType() == 1) {
    pushHKTelemetry();
  }
  else if (interpreter_->CurrentTelemetryType() == 2) {
    pushWFTelemetry();
  }
  else if (interpreter_->CurrentTelemetryType() == 3) {
    pushStatusTelemetry();
  }
  return AS_OK;
}

void PushToMongoDB::pushHKTelemetry() {
  TelemetryDefinition *telemdef = interpreter_->Telemdef();

  hsquicklook::DocumentBuilder builder("Telemetry", "HK");
  builder.setTI(telemdef->TimeNow().tv_sec * 64 + telemdef->TimeNow().tv_usec * 64 * 1E-6);
  builder.setTimeNow();

  {
    const std::string section_name = "Header";
    auto section = bsoncxx::builder::stream::document{}
                   << "Start_Code" << static_cast<long long>(telemdef->StartCode())
                   << "Telemetry_Type" << static_cast<int>(telemdef->TelemetryType())
                   << "Time" << static_cast<int>((telemdef->TimeNow()).tv_sec)
                   << "Time_us" << static_cast<int>((telemdef->TimeNow()).tv_usec)
                   << "Telemetry_Index" << static_cast<int>(telemdef->TelemetryIndex())
                   << "Run_ID" << static_cast<int>(telemdef->RunID())
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  {
    const std::string section_name = "Detector";
    auto section = bsoncxx::builder::stream::document{}
                   << "Event_Count" << static_cast<int>(telemdef->EventCount())
                   << "Event_Rate" << telemdef->EventRate()
                   << "Current_EventID" << static_cast<int>(telemdef->CurrentEventID())
                   << "Chamber_Pressure" << static_cast<int>(telemdef->ChamberPressure())
                   << "Chamber_Temperature_1" << static_cast<int>(telemdef->ChamberTemperature(0))
                   << "Chamber_Temperature_2" << static_cast<int>(telemdef->ChamberTemperature(1))
                   << "Chamber_Temperature_3" << static_cast<int>(telemdef->ChamberTemperature(2))
                   << "Valve_Temperature" << static_cast<int>(telemdef->ValveTemperature())
                   << "Outer_Temperature" << static_cast<int>(telemdef->OuterTemperature())
                   << "TPC_High_Voltage_Setting" << static_cast<double>(telemdef->TPCHVSetting())
                   << "TPC_High_Voltage_Measurement" << static_cast<int>(telemdef->TPCHVMeasure())
                   << "PMT_High_Voltage_Setting" << static_cast<double>(telemdef->PMTHVSetting())
                   << "TPC_High_Voltage_Current_Measurement" << static_cast<int>(telemdef->TPCHVCurrentMeasure())
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  {
    const std::string section_name = "DAQ_Vessel";
    auto section_stream = bsoncxx::builder::stream::document{};
    section_stream
        << "CPU_Temperature" << telemdef->CPUTemperature();
    for (int i = 0; i < 5; i++) {
      const int k = i + 1;
      section_stream << (boost::format("HK_Temperature_%d") % k).str() << telemdef->EnvTemperature(i);
      section_stream << (boost::format("HK_Humidity_%d") % k).str() << telemdef->EnvHumidity(i);
      section_stream << (boost::format("HK_Pressure_%d") % k).str() << telemdef->EnvPressure(i);
    }
    for (int i = 0; i < 3; i++) {
      const char c = 'x' + i;
      section_stream << (boost::format("Acceleration_%s") % c).str() << telemdef->Acceleration(i);
      section_stream << (boost::format("Gyro_%s") % c).str() << telemdef->Gyro(i);
      section_stream << (boost::format("Magnet_%s") % c).str() << telemdef->Magnet(i);
    }
    section_stream
        << "Accel_Sensor_Temperature" << telemdef->AccelSensorTemperature()
        << "Main_Current" << static_cast<int>(telemdef->MainCurrent())
        << "Main_Voltage" << static_cast<int>(telemdef->MainVoltage());
    auto section = section_stream << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  {
    const std::string section_name = "Software";
    auto section = bsoncxx::builder::stream::document{}
                   << "Last_Command_Index" << static_cast<int>(telemdef->LastCommandIndex())
                   << "Last_Command_Code" << static_cast<int>(telemdef->LastCommandCode())
                   << "Command_Reject_Count" << static_cast<int>(telemdef->CommandRejectCount())
                   << "Software_Error_Code" << static_cast<long long>(telemdef->SoftwareErrorCode())
                   << "CRC" << static_cast<int>(telemdef->CRC())
                   << "Stop_Code" << static_cast<long long>(telemdef->StopCode())
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }
  {
    const std::string section_name = "Software_Error";
    auto section_stream = bsoncxx::builder::stream::document{};
    uint64_t error_code = telemdef->SoftwareErrorCode();
    for (int i = 0; i < 64; i++) {
      const std::string error_name = ErrorManager::bitToStr(i);
      if (error_name == "") {
        continue;
      }
      if ((error_code >> i) & 1) {
        section_stream << error_name << "Error";
      }
      else {
        section_stream << error_name << "OK";
      }
    }
    auto section = section_stream << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  auto doc = builder.generate();
  mongodbClient_->push("grams", doc);
}

void PushToMongoDB::pushWFTelemetry() {
  TelemetryDefinition *telemdef = interpreter_->Telemdef();

  hsquicklook::DocumentBuilder builder("Telemetry", "WF");
  builder.setTI(telemdef->TimeNow().tv_sec * 64 + telemdef->TimeNow().tv_usec * 64 * 1E-6);
  builder.setTimeNow();

  {
    const std::string section_name = "Header";
    auto section = bsoncxx::builder::stream::document{}
                   << "Start_Code" << static_cast<long long>(telemdef->StartCode())
                   << "Telemetry_Type" << static_cast<int>(telemdef->TelemetryType())
                   << "Time" << static_cast<int>((telemdef->TimeNow()).tv_sec)
                   << "Time_us" << static_cast<int>((telemdef->TimeNow()).tv_usec)
                   << "Telemetry_Index" << static_cast<int>(telemdef->TelemetryIndex())
                   << "Run_ID" << static_cast<int>(telemdef->RunID())
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  {
    const std::string section_name = "Event";
    auto section = bsoncxx::builder::stream::document{}
                   << "Event_ID" << static_cast<int>(telemdef->EventID())
                   << "Event_Time" << static_cast<int>((telemdef->EventTime()).tv_sec)
                   << "Event_Time_us" << static_cast<int>((telemdef->EventTime()).tv_usec)
                   << "Sample_Frequency" << telemdef->SampleFrequency()
                   << "Time_Window" << telemdef->TimeWindow()
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  {
    const std::string section_name = "Footer";
    auto section = bsoncxx::builder::stream::document{}
                   << "CRC" << static_cast<int>(telemdef->CRC())
                   << "Stop_Code" << static_cast<long long>(telemdef->StopCode())
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  auto doc = builder.generate();
  mongodbClient_->push("grams", doc);
}

void PushToMongoDB::pushStatusTelemetry() {
  TelemetryDefinition *telemdef = interpreter_->Telemdef();

  hsquicklook::DocumentBuilder builder("Telemetry", "Status");
  builder.setTI(telemdef->TimeNow().tv_sec * 64 + telemdef->TimeNow().tv_usec * 64 * 1E-6);
  builder.setTimeNow();

  {
    const std::string section_name = "Header";
    auto section = bsoncxx::builder::stream::document{}
                   << "Start_Code" << static_cast<long long>(telemdef->StartCode())
                   << "Telemetry_Type" << static_cast<int>(telemdef->TelemetryType())
                   << "Time" << static_cast<int>((telemdef->TimeNow()).tv_sec)
                   << "Time_us" << static_cast<int>((telemdef->TimeNow()).tv_usec)
                   << "Telemetry_Index" << static_cast<int>(telemdef->TelemetryIndex())
                   << "Run_ID" << static_cast<int>(telemdef->RunID())
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  {
    const std::string section_name = "Status";
    auto section = bsoncxx::builder::stream::document{}
                   << "Trigger_Mode" << static_cast<int>(telemdef->TriggerMode())
                   << "Trigger_Device" << static_cast<int>(telemdef->TriggerDevice())
                   << "Trigger_Channel" << static_cast<int>(telemdef->TriggerChannel())
                   << "Trigger_Level" << telemdef->TriggerLevel()
                   << "Trigger_Position" << telemdef->TriggerPosition()
                   << "Sample_Frequency" << telemdef->SampleFrequency()
                   << "Time_Window" << telemdef->TimeWindow()
                   << "ADC_Offset_1" << (telemdef->ADCOffset())[0]
                   << "ADC_Offset_2" << (telemdef->ADCOffset())[1]
                   << "ADC_Offset_3" << (telemdef->ADCOffset())[2]
                   << "ADC_Offset_4" << (telemdef->ADCOffset())[3]
                   << "ADC_Range_1" << (telemdef->ADCRange())[0]
                   << "ADC_Range_2" << (telemdef->ADCRange())[1]
                   << "ADC_Range_3" << (telemdef->ADCRange())[2]
                   << "ADC_Range_4" << (telemdef->ADCRange())[3]
                   << "DAQ_In_Progress" << static_cast<int>(telemdef->DAQInProgress())
                   << "TPC_HV_Upper_Limit" << telemdef->TPCHVUpperLimit()
                   << "PMT_HV_Upper_Limit" << telemdef->PMTHVUpperLimit()
                   << "SD_Capacity" << (static_cast<double>(telemdef->SDCapacity()) / std::pow(2.0, 30.0))
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  {
    const std::string section_name = "Footer";
    auto section = bsoncxx::builder::stream::document{}
                   << "CRC" << static_cast<int>(telemdef->CRC())
                   << "Stop_Code" << static_cast<long long>(telemdef->StopCode())
                   << bsoncxx::builder::stream::finalize;
    builder.addSection(section_name, section);
  }

  auto doc = builder.generate();
  mongodbClient_->push("grams", doc);
}

void PushToMongoDB::pushWaveformImage(const std::vector<std::string> &keys, const std::vector<std::string> &image_filenames) {
  if (keys.size() != image_filenames.size()) {
    std::cerr << "Error in PushToMongoDB::pushWaveformImage  keys and image_filenames should be the same size." << std::endl;
    return;
  }

  TelemetryDefinition *telemdef = interpreter_->Telemdef();
  hsquicklook::DocumentBuilder builder("Telemetry", "waveform_image");
  builder.setTI(telemdef->TimeNow().tv_sec * 64 + telemdef->TimeNow().tv_usec * 64 * 1E-6);
  builder.setTimeNow();

  const std::size_t size = 10 * 1024 * 1024;
  static uint8_t buf[size];
  const int n = keys.size();
  for (int i = 0; i < n; i++) {
    const std::string key = keys[i];
    const std::string image_filename = image_filenames[i];
    std::ifstream fin(image_filename.c_str(), std::ios::in | std::ios::binary);
    fin.read((char *)buf, size);
    const std::size_t readSize = fin.gcount();
    fin.close();

    {
      const std::string section_name = key;
      auto section = bsoncxx::builder::stream::document{}
                     << key << hsquicklook::make_image_value(buf, readSize, 640, 480, image_filename)
                     << bsoncxx::builder::stream::finalize;
      builder.addSection(section_name, section);
    }
  }
  auto doc = builder.generate();
  mongodbClient_->push("grams", doc);
}

} /* namespace gramsballoon */
