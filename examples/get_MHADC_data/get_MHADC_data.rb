#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'
MHz = 1000000
class MyApp < ANL::ANLApp
  def setup()
    chain GRAMSBalloon::EncodedSerialCommunicator
    with_parameters(filename: "/dev/ttyACM1", baudrate:15, chatter:100, timeout_sec: 0, timeout_usec: 10)
    chain GRAMSBalloon::GetMHADCData
    with_parameters(num_ch: 32, chatter: 100, sleep_for_msec:1)
    chain GRAMSBalloon::MeasureTemperatureWithRTDSensorByMHADC, "MeasureTemperatureWithRTDSensorByMHADC_#{0}"
    with_parameters(channel: 0, chatter: 1) do |m|
            m.set_singleton(0)
    end
    chain GRAMSBalloon::MeasureOrientationByMHADC
    with_parameters(chatter: 3, sleep_for_msec: 1, num_trials:10)
  end
end

a = MyApp.new
a.run(1, 1)
