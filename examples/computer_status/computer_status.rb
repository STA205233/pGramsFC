#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'

class MyApp < ANL::ANLApp
  def setup()
    chain GRAMSBalloon::GetComputerStatus
    with_parameters(chatter: 4)
  end
end

a = MyApp.new
a.run(100, 1)
