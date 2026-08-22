#! /usr/bin/env ruby
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
    sendCommandToDAQComputer_names = []
    chain GRAMSBalloon::ComMosquittoManager
    with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, threaded_set: true, device_id: "hubcomputer_c", time_out: 1) do |m|
      m.set_singleton(1)
    end
    chain GRAMSBalloon::IoContextManager do |m|
      m.set_singleton(0)
    end
    @main_modules << "IoContextManager"
    chain GRAMSBalloon::ControlToFBias
    with_parameters(path: "/dev/ttyUSB0", chatter: 0, minimum_duration_sec:3600) do |m|
      m.set_singleton(0)
    end
    @main_modules << "ControlToFBias"
    chain GRAMSBalloon::ReceiveCommand
    with_parameters(topic: @inifile["Hub"]["comtopic"], chatter: 5, qos: 0, binary_filename_base: "command", SendCommandToDAQComputer_names: sendCommandToDAQComputer_names) do |m|
      m.set_singleton(0)
    end
    @main_modules << "ReceiveCommand"
    chain GRAMSBalloon::Sleep
    with_parameters(sleep_sec: 1) do |m|
      m.set_singleton(0)
    end 
    @main_modules << "Sleep"
  end
end

a = MyApp.new
a.num_parallels = 2
mosquitto_modules = ["ComMosquittoManager"]
a.modify do |m|
  a.main_modules.each do |mod|
   m.get_parallel_module(1, mod).off
  end
  mosquitto_modules.each do |mod|
   m.get_parallel_module(0, mod).off
  end
end
a.setup()
a.run(:all, 100000000000000)