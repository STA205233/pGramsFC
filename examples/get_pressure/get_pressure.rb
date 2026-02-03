#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'
class MyApp < ANL::ANLApp
  def setup()
    chain GRAMSBalloon::PressureGaugeManager
    with_parameters(filename: "/dev/ttyUSB1", baudrate:115200, timeout_sec: 0, timeout_usec:1)
    chain GRAMSBalloon::GetPressure
    with_parameters(channel: 1, EncodedSerialCommunicator_name: "PressureGaugeManager", sleep_for_usec: 10, num_sent_command_per_trial: 2, chatter: 1, num_trials: 1)
  end
end

a = MyApp.new
a.run(100, 1)
