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
        chain GRAMSBalloon::ComMosquittoManager
        with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, threaded_set: true, device_id: "hubcomputer_c", time_out: 100) do |m|
        m.set_singleton(0)
        end
        chain GRAMSBalloon::ReceiveCommand
        with_parameters(topic: @inifile["Hub"]["comtopic"], chatter: 100, qos: 0, binary_filename_base: "command_test") do |m|
        m.set_singleton(0)
        end
        chain GRAMSBalloon::ShutdownSystem
    end
end

a = MyApp.new
a.run(:all, 1)
