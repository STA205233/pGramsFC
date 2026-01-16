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
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, threaded_set: true, device_id: "hubcomputer_t", time_out: 100) do |m|
      m.set_singleton(0)
    end
    chain GRAMSBalloon::ComMosquittoManager
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, threaded_set: true, device_id: "hubcomputer_c", time_out: 100) do |m|
      m.set_singleton(0)
    end
    chain GRAMSBalloon::IoContextManager do |m|
      m.set_singleton(0)
    end
    chain GRAMSBalloon::ReceiveCommand
    with_parameters(topic: @inifile["Hub"]["comtopic"], chatter: 100, qos: 0, binary_filename_base: "command_test") do |m|
      m.set_singleton(0)
    end
    chain GRAMSBalloon::EncodedSerialCommunicator,"MHADCManager"
    with_parameters(filename: "/dev/ttyACM0", baudrate:15, chatter:100, timeout_sec: 10, timeout_usec: 10)
    #chain GRAMSBalloon::GetMHADCData
    #with_parameters(MHADCManager_name: "MHADCManager",channel_per_section: 6, num_section: 8, chatter: 100, sleep_for_msec:1)
    chain GRAMSBalloon::GetComputerStatus
    with_parameters(chatter: 1)
    chain GRAMSBalloon::SendTelemetry
    with_parameters(
          topics: [@inifile["Hub"]["teltopic"]],
          qos:0,
          save_telemetry: false,
          binary_filename_base: "telemetry_test",
          num_telem_per_file: 1000,
          chatter: 0,
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

