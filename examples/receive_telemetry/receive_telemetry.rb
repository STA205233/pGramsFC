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
    chain GRAMSBalloon::MosquittoManager
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, threaded_set: true, device_id: "ground_system")
    chain GRAMSBalloon::ReceiveTelemetry, "ReceiveTelemetry_TPC"
    with_parameters(topic: @inifile["TPC"]["teltopic"], qos: 0, chatter: 100)
    chain GRAMSBalloon::InterpretDAQFormattedTelemetry, "InterpretDAQFormattedTelemetry_TPC"
    with_parameters(ReceiveTelemetry_name: "ReceiveTelemetry_TPC", print_telemetry:true, chatter: 100)
    chain GRAMSBalloon::Sleep
    with_parameters(sleep_sec: 1)
  end
end

a = MyApp.new


a.num_parallels = 1
a.run(1000000000, 1)
exit_status = 1
puts "exit_status: #{exit_status}"
exit exit_status

