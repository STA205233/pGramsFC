#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'
require 'inifile'

class MyApp < ANL::ANLApp
  attr_accessor :inifile
  def setup()
    @inifile = IniFile.load('../../settings/network.cfg')
    if inifile.nil?
      puts "Error: network.cfg not found"
      exit 1
    end
    chain GRAMSBalloon::TelemMosquittoManager, "TelemMosquittoManager"
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 10, chatter: 0, threaded_set: true, device_id: "Ground") do |m|
      m.set_singleton(0)
    end
    chain GRAMSBalloon::ComMosquittoManager, "GroundMosquittoManager"
    with_parameters(host: "localhost", port: 1883, keep_alive: 10, chatter: 0, threaded_set: true, device_id: "Ground_hub")
    subsystems = ["Orchestrator", "TPC", "TOF"]
    for subsystem in subsystems
      chain GRAMSBalloon::ReceiveTelemetry, "ReceiveTelemetry_#{subsystem}"
      with_parameters(topic: @inifile[subsystem]["teltopic"], chatter: 0)
      chain GRAMSBalloon::InterpretTelemetry, "InterpretBaseTelemetry_#{subsystem}"
      with_parameters(receiver_module_name: "ReceiveTelemetry_#{subsystem}", chatter: 2, telemetry_type: "Base")
      chain GRAMSBalloon::ReceiveTelemetry, "ReceiveTelemetry_#{subsystem}_Iridium"
      with_parameters(topic: @inifile[subsystem]["iridiumteltopic"], chatter: 4)
      chain GRAMSBalloon::InterpretTelemetry, "InterpretBaseTelemetry_#{subsystem}_Iridium"
      with_parameters(receiver_module_name: "ReceiveTelemetry_#{subsystem}_Iridium", chatter: 2, telemetry_type: "Base")
      chain GRAMSBalloon::SendArrayByMQTT, "SendArrayByMQTT_#{subsystem}_Iridium"
      with_parameters(InterpretTelemetry_name: "InterpretBaseTelemetry_#{subsystem}_Iridium", MosquittoManager_name: "GroundMosquittoManager", topic: "#{subsystem}_ground_telemetry", qos: 0)
      chain GRAMSBalloon::SendArrayByMQTT, "SendArrayByMQTT_#{subsystem}"
      with_parameters(InterpretTelemetry_name: "InterpretBaseTelemetry_#{subsystem}", MosquittoManager_name: "GroundMosquittoManager", topic: "#{subsystem}_ground_telemetry", qos: 0)
    end
    chain GRAMSBalloon::ReceiveTelemetry, "ReceiveTelemetry_HK"
    with_parameters(topic: @inifile["Hub"]["iridiumteltopic"], chatter: 0)
    chain GRAMSBalloon::InterpretTelemetry, "InterpretHKTelemetry"
    with_parameters(receiver_module_name: "ReceiveTelemetry_HK", run_ID_filename: ENV["HOME"] + "/settings/run_id/run_id_ground.txt", save_telemetry: false, num_telem_per_file: 1000, chatter: 2, telemetry_type: "HK")
    chain GRAMSBalloon::SendArrayByMQTT, "SendArrayByMQTT_HK"
      with_parameters(InterpretTelemetry_name: "InterpretHKTelemetry", MosquittoManager_name: "GroundMosquittoManager", topic: "HK_ground_telemetry", qos: 0, chatter: 0)
    chain GRAMSBalloon::PushToMySQL
    with_parameters(HubHKInterpreter_name: "InterpretHKTelemetry", host: ENV["PGRAMS_MYSQL_HOST"], user: ENV["PGRAMS_MYSQL_USER"], password: ENV["PGRAMS_MYSQL_PASSWD"], port: ENV["PGRAMS_MYSQL_PORT"].to_i, database: "pgrams", check_exist: true, chatter: 0)
  end
end

a = MyApp.new
a.run(:all)
