#!/usr/bin/env ruby
require 'anlnext'
require 'GRAMSBalloon'
require 'inifile'

class MyApp < ANL::ANLApp
  attr_accessor :inifile, :main_modules
  
  def setup()
    @inifile = IniFile.load('../../settings/network.cfg')
    if inifile.nil?
      puts "Error: network.cfg not found"
      exit 1
    end
    @main_modules = []
    chain GRAMSBalloon::TelemMosquittoManager
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, device_id: "hubcomputer_t", time_out: 1, do_initialize: true) do |m|
      m.set_singleton(0)
    end

    chain GRAMSBalloon::ComMosquittoManager
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, device_id: "hubcomputer_c", time_out: 1, do_cleanup: true) do |m|
      m.set_singleton(0)
    end
    
    chain GRAMSBalloon::SetHKEvs
    with_parameters(duration_msec: 1000, chatter: 0) do |m|
      m.set_singleton(0)
    end
    @main_modules << "SetHKEvs"
    
    chain GRAMSBalloon::IoContextManager do |m|
      m.set_singleton(0)
    end
    @main_modules << "IoContextManager"
    
    
    chain GRAMSBalloon::SPIManager, "SPIManager_baycat"
    with_parameters(channel: 0, spi_config_options: 2, spi_control_type: "baycat", use_multiplexer: true) do |m|
      m.set_singleton(0)
    end
    @main_modules << "SPIManager_baycat"
    chain GRAMSBalloon::ControlPDU
    with_parameters(SPIManager_name: "SPIManager_baycat") do |m|
      m.set_singleton(0)
    end
    @main_modules << "ControlPDU"
    chain GRAMSBalloon::GetPDUInfo
    with_parameters(SPIManager_name: "SPIManager_baycat", chatter: 0, v_ref: 5.0) do |m|
      m.set_singleton(0)
    end
    @main_modules << "GetPDUInfo"
    
    
    subsystems = ["TPC", "TOF", "Orchestrator", "TPCMonitor"]
    subsystem_overwritten={"TPC"=>0, "TPCMonitor"=>0,"TOF"=>0, "Orchestrator"=>12320}
    subsystemInts = {"Hub" => 0, "TPC" => 2, "TPCMonitor"=> 3,"TOF" => 4, "Orchestrator" => 1}
    # 0x4004/0x4005 are full-event FEM headers + complete marker (Starlink-only, large packets).
    subsystem_starlink={"TPCMonitor"=>[0x4002, 0x4003, 0x4004, 0x4005], "TPC" => [],"TOF" => [], "Orchestrator" => []}
    subsystem_dead_com_time={"TPCMonitor"=>0, "Orchestrator"=>5000, "TOF"=>0, "TPC"=>0}
    sendCommandToDAQComputer_names = []
    subsystems.each do |subsystem|
      sendCommandToDAQComputer_names << "SendCommandToDAQComputer_" + subsystem # SendCommandToDAQComputer_names is used for only emergency_DAQ_shutdown.
    end
    comms = ["iridium", ""]
    
    comms.each do |com|
      chain GRAMSBalloon::ReceiveCommand, "ReceiveCommand#{com}"
      with_parameters(topic: @inifile["Hub"]["#{com}comtopic"], chatter: 0, qos: 0, binary_filename_base: "command/command", SendCommandToDAQComputer_names: sendCommandToDAQComputer_names) do |m|
        m.set_singleton(0)
      end
      @main_modules << "ReceiveCommand#{com}"
    end
    
    subsystems.each do |subsystem|
      chain GRAMSBalloon::SocketCommunicationManager, "SocketCommunicationManager_" + subsystem
      with_parameters(ip: @inifile[subsystem]["ip"], port: @inifile[subsystem]["comport"].to_i, subsystem: subsystemInts[subsystem], timeout: 100, chatter: 0) do |m|
        m.set_singleton(0)
      end
      @main_modules << "SocketCommunicationManager_" + subsystem
      
      chain GRAMSBalloon::SocketCommunicationManager, "SocketCommunicationManager_#{subsystem}_rsv"
      with_parameters(ip: @inifile[subsystem]["ip"], port: @inifile[subsystem]["telport"].to_i, timeout: 100, subsystem: subsystemInts[subsystem], chatter: 0) do |m|
        m.set_singleton(0)
      end
      @main_modules << "SocketCommunicationManager_#{subsystem}_rsv"
      
      comms.each do |com|
        chain GRAMSBalloon::DistributeCommand, "DistributeCommand_#{subsystem}#{com}"
        with_parameters(topic: @inifile[subsystem]["#{com}comtopic"], chatter: 1) do |m|
          m.set_singleton(0)
        end
        @main_modules << "DistributeCommand_#{subsystem}#{com}"
        if com == "" 
          duration = 1000
        else 
          duration = -1
        end
        chain GRAMSBalloon::SendCommandToDAQComputer, "SendCommandToDAQComputer_" + subsystem + com
          with_parameters(SocketCommunicationManager_name: "SocketCommunicationManager_#{subsystem}", duration_between_heartbeat: duration, DistributeCommand_name: "DistributeCommand_#{subsystem}#{com}", subsystem: subsystemInts[subsystem], chatter: 0) do |m|
          m.set_singleton(0)
        end
        @main_modules << "SendCommandToDAQComputer_" + subsystem + com
      end
      
      chain GRAMSBalloon::ReceiveStatusFromDAQComputer, "ReceiveStatusFromDAQComputer_" + subsystem
        with_parameters(SocketCommunicationManager_name:"SocketCommunicationManager_#{subsystem}_rsv", dead_communication_time: subsystem_dead_com_time[subsystem], subsystem: subsystemInts[subsystem],chatter: 0) do |m|
        m.set_singleton(0)
      end
      @main_modules << "ReceiveStatusFromDAQComputer_" + subsystem
      
      chain GRAMSBalloon::DividePacket, "DividePacket_#{subsystem}"
        with_parameters(ReceiveStatusFromDAQComputer_name: "ReceiveStatusFromDAQComputer_#{subsystem}", starlink_code: subsystem_starlink[subsystem], overwritten_packet_code: subsystem_overwritten[subsystem], chatter: 0) do |m|
        m.set_singleton(0)
      end
      @main_modules << "DividePacket_#{subsystem}"
      
      chain GRAMSBalloon::PassTelemetry, "PassTelemetry_#{subsystem}_starlink"
        with_parameters(DividePacket_name: "DividePacket_#{subsystem}", topic: @inifile[subsystem]["iridiumteltopic"], starlink_topic:@inifile[subsystem]["teltopic"], is_starlink_only: true, chatter: 0) do |m|
        m.set_singleton(0)
      end
      @main_modules << "PassTelemetry_#{subsystem}_starlink"
      
      chain GRAMSBalloon::PassTelemetry, "PassTelemetry_#{subsystem}_iridium"
        with_parameters(DividePacket_name: "DividePacket_#{subsystem}", topic: @inifile[subsystem]["iridiumteltopic"], starlink_topic:@inifile[subsystem]["teltopic"], is_starlink_only: false, chatter: 0) do |m|
        m.set_singleton(0)
      end
      @main_modules << "PassTelemetry_#{subsystem}_iridium"
    end
    
    chain GRAMSBalloon::EncodedSerialCommunicator, "MHADCManager"
    with_parameters(filename: "/dev/ttyACM0", baudrate:15, chatter: 0, timeout_usec: 1000) do |m|
      m.set_singleton(0)
    end
    @main_modules << "MHADCManager"
    
    chain GRAMSBalloon::GetMHADCData
    with_parameters(MHADCManager_name: "MHADCManager", channel_per_section: 6, num_section:8, chatter: 3) do |m|
      m.set_singleton(0)
    end
    @main_modules << "GetMHADCData"
    
    chain GRAMSBalloon::GetComputerStatus  do |m|
      m.set_singleton(0)
    end
    @main_modules << "GetComputerStatus"
    
    chain GRAMSBalloon::ControlToFBias
    with_parameters(path: "/dev/ttyUSB0", timeout_usec: 100000, MosquittoManager_name: "TelemMosquittoManager", topic: @inifile["TOFBias"]["iridiumteltopic"], starlink_topic: @inifile["TOFBias"]["teltopic"], chatter: 4, minimum_duration_sec: 10) do |m|
      m.set_singleton(0)
    end
    @main_modules << "ControlToFBias"
    
    chain GRAMSBalloon::SendTelemetry
    with_parameters(
          topic: @inifile["Hub"]["iridiumteltopic"],
          starlink_topic: @inifile["Hub"]["teltopic"],
          qos:0,
          save_telemetry: false,
          binary_filename_base: "telemetry/telemetry",
          num_telem_per_file: 10000,
          chatter: 0,
    ) do |m|
      m.set_singleton(0)
    end
    @main_modules << "SendTelemetry"

    chain GRAMSBalloon::RunIDManager
    with_parameters(
      filename: ENV["HOME"] + "/settings/run_id/run_id.txt"
    ) do |m|
        m.set_singleton(0)
    end
    @main_modules << "RunIDManager"
  end
end

a = MyApp.new


a.num_parallels = 1
mosquitto_modules = ["TelemMosquittoManager", "ComMosquittoManager"]
a.modify do |m|
  #a.main_modules.each do |mod|
  # m.get_parallel_module(1, mod).off
  #end
  #mosquitto_modules.each do |mod|
  # m.get_parallel_module(0, mod).off
  #end
end
#a.run(1, 1)
a.run(:all, 1000000000)
exit_status = 1
puts "exit_status: #{exit_status}"
exit exit_status

