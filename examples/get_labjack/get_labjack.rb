#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'
class MyApp < ANL::ANLApp
  def setup()
    chain GRAMSBalloon::GetLabJackData
    with_parameters(chatter: 0)
  end
end

a = MyApp.new
a.run(:all, 1)
