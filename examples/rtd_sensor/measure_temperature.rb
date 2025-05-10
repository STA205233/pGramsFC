#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'

class MyApp < ANL::ANLApp
    def setup()
        chain GRAMSBalloon::SPIManager
        with_parameters(channel: 0, baudrate:1000000, spi_config_options: 3+0+32)
        chain GRAMSBalloon::MeasureTemperatureWithRTDSensor
        with_parameters(chip_select: 8)
    end
end

a = MyApp.new
a.run(100, 1)
