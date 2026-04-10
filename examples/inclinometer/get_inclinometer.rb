#!/usr/bin/env ruby
require 'anlnext'
require 'GRAMSBalloon'
require 'inifile'

class MyApp < ANL::ANLApp
  attr_accessor :inifile
  def setup()
    chain GRAMSBalloon::GetInclinometerData
    with_parameters(chatter: 4, timeout_sec: 0, baudrate: 9, serial_path:"/tmp/ttyClient")
    chain GRAMSBalloon::Sleep
    with_parameters(sleep_sec: 1)
  end
end  
a = MyApp.new
a.run(1000, 1)
