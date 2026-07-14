#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'

class MyApp < ANL::ANLApp
    def setup()
        chain GRAMSBalloon::I2CManager
        chain GRAMSBalloon::GetEnvironmentalData
        with_parameters(chatter:2)
    end
end

a = MyApp.new
a.run(100, 1)
