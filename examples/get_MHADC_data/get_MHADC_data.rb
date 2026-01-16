#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'
MHz = 1000000
class MyApp < ANL::ANLApp
  def setup()
    chain GRAMSBalloon::EncodedSerialCommunicator
    with_parameters(filename: "/dev/ttyACM0", baudrate:15, chatter:100, timeout_sec: 10, timeout_usec: 10)
    chain GRAMSBalloon::GetMHADCData
    with_parameters(channel_per_section: 6, num_section:8, chatter: 100, sleep_for_msec:1)
    chain GRAMSBalloon::MeasureTemperatureWithRTDSensorByMHADC, "MeasureTemperatureWithRTDSensorByMHADC_#{0}"
    with_parameters(channel: 0, chatter: 1) do |m|
            m.set_singleton(0)
    end
    chain GRAMSBalloon::MeasureTemperatureWithRTDSensorByMHADC, "MeasureTemperatureWithRTDSensorByMHADC_#{1}"
    with_parameters(channel: 3, chatter: 1) do |m|
            m.set_singleton(0)
    end
    #chain GRAMSBalloon::MeasureOrientationByMHADC
    #with_parameters(chatter: 3, sleep_for_msec: 1, num_trials:10)
  end
end

a = MyApp.new
a.run(:all, 1)
