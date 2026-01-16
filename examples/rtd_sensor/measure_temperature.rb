#!/usr/bin/env ruby

require 'anlnext'
require 'GRAMSBalloon'

class MyApp < ANL::ANLApp
    def setup()
        chain GRAMSBalloon::SPIManager
        with_parameters(channel: 16, baudrate: 750000, spi_config_options: 1+0)
        chain GRAMSBalloon::MeasureTemperatureWithRTDSensor
        with_parameters(chip_select: 16)
    end
end

a = MyApp.new
a.run(1000, 1)
