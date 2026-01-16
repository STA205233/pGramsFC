#!/usr/bin/env ruby
require 'anlnext'
require 'GRAMSBalloon'

class MyApp < ANL::ANLApp
    def setup()
      chain GRAMSBalloon::MosquittoManager
      with_parameters(host: ENV["PGRAMS_MOSQUITTO_HOST"], port: ENV["PGRAMS_MOSQUITTO_PORT"].to_i, password: ENV["PGRAMS_MOSQUITTO_PASSWD"], user: ENV["PGRAMS_MOSQUITTO_USER"], keep_alive: 60, chatter: 0, threaded_set: true, device_id: "hubcomputer") do |m|
        m.set_singleton(0)
      end
      chain GRAMSBalloon::ReceiveCommand
      with_parameters(topic: "command", chatter: 100, qos: 0, binary_filename_base: ENV["HOME"]+"/command_test/command") do |m|
        m.set_singleton(0)
      end
      chain GRAMSBalloon::DistributeCommand
      with_parameters(chatter: 0) do |m|
        m.set_singleton(0)
      end
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

