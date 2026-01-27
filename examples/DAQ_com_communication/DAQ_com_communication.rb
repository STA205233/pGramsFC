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
    chain GRAMSBalloon::TelemMosquittoManager
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, threaded_set: true, device_id: "hubcomputer_t", time_out: 1) do |m|
      m.set_singleton(0)
    end
    chain GRAMSBalloon::ComMosquittoManager
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, threaded_set: true, device_id: "hubcomputer_c", time_out: 1) do |m|
      m.set_singleton(0)
    end
    chain GRAMSBalloon::IoContextManager do |m|
      m.set_singleton(0)
    end
    #subsystems = ["Orchestrator"]
    subsystems = ["TPC", "TOF", "Orchestrator"]
    subsystemInts = {"Hub" => 0, "TPC" => 2, "QM"=> 3,"TOF" => 4, "Orchestrator" => 1}
    sendCommandToDAQComputer_names = []
    subsystems.each do |subsystem|
      sendCommandToDAQComputer_names << "SendCommandToDAQComputer_" + subsystem
    end
    chain GRAMSBalloon::ReceiveCommand
    with_parameters(topic: @inifile["Hub"]["comtopic"], chatter: 100, qos: 0, binary_filename_base: "command_test", SendCommandToDAQComputer_names: sendCommandToDAQComputer_names) do |m|
      m.set_singleton(0)
    end
    subsystems.each do |subsystem|
      chain GRAMSBalloon::SocketCommunicationManager, "SocketCommunicationManager_" + subsystem
      with_parameters(ip: @inifile[subsystem]["ip"], port: @inifile[subsystem]["comport"].to_i, timeout: 100, chatter: 
      1000) do |m|
        m.set_singleton(0)
      end
      chain GRAMSBalloon::SocketCommunicationManager, "SocketCommunicationManager_#{subsystem}_rsv"
      with_parameters(ip: @inifile[subsystem]["ip"], port: @inifile[subsystem]["telport"].to_i, timeout: 100, chatter: 1000) do |m|
        m.set_singleton(0)
      end
      chain GRAMSBalloon::DistributeCommand, "DistributeCommand_#{subsystem}"
      with_parameters(topic: @inifile[subsystem]["comtopic"], chatter: 2) do |m|
        m.set_singleton(0)
      end
      chain GRAMSBalloon::SendCommandToDAQComputer, "SendCommandToDAQComputer_" + subsystem
        with_parameters(SocketCommunicationManager_name: "SocketCommunicationManager_#{subsystem}", duration_between_heartbeat: 1000, DistributeCommand_name: "DistributeCommand_#{subsystem}", subsystem: subsystemInts[subsystem], chatter: 4)
      chain GRAMSBalloon::ReceiveStatusFromDAQComputer, "ReceiveStatusFromDAQComputer_" + subsystem
        with_parameters(SocketCommunicationManager_name:"SocketCommunicationManager_#{subsystem}_rsv", dead_communication_time: 1000,chatter: 0)
      chain GRAMSBalloon::DividePacket, "DividePacket_#{subsystem}"
        with_parameters(ReceiveStatusFromDAQComputer_name: "ReceiveStatusFromDAQComputer_#{subsystem}", starlink_code: [64], overwritten_packet_code: 331, chatter: 10)
      chain GRAMSBalloon::PassTelemetry, "PassTelemetry_#{subsystem}_starlink"
        with_parameters(DividePacket_name: "DividePacket_#{subsystem}", topic: @inifile[subsystem]["iridiumteltopic"], starlink_topic:@inifile[subsystem]["teltopic"], is_starlink: true, chatter: 0)
      chain GRAMSBalloon::PassTelemetry, "PassTelemetry_#{subsystem}_iridium"
        with_parameters(DividePacket_name: "DividePacket_#{subsystem}", topic: @inifile[subsystem]["iridiumteltopic"], is_starlink: false, chatter: 0)
    end
    chain GRAMSBalloon::DetectErrorCallbackFromDAQ, "DetectErrorCallbackFromDAQ_TOF"
    with_parameters(DividePacket_name: "DividePacket_TOF", chatter: 2) do |m|
      m.set_singleton(0)
    end
    chain GRAMSBalloon::SendTelemetry
    with_parameters(
          topic: @inifile["Hub"]["iridiumteltopic"],
          starlink_topic: @inifile["Hub"]["teltopic"],
          qos:0,
          save_telemetry: false,
          binary_filename_base: "telemetry_test",
          num_telem_per_file: 1000,
          chatter: 2,
    )
    chain GRAMSBalloon::RunIDManager
    with_parameters(
      filename: ENV["HOME"] + "/settings/run_id/run_id.txt"
    ) do |m|
        m.set_singleton(0)
    end
    chain GRAMSBalloon::Sleep
    with_parameters(
      sleep_sec: 1
    )
  end
end

a = MyApp.new


a.num_parallels = 1
#a.run(1, 1)
a.run(1000000000, 1000000000)
exit_status = 1
puts "exit_status: #{exit_status}"
exit exit_status

