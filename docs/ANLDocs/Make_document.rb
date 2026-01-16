#!/usr/bin/env ruby

require 'GRAMSBalloon'
require 'anlnext'

a = ANL::AnalysisChain.new
include GRAMSBalloon
a.push EncodedSerialCommunicator.new

a.push GetMHADCData.new

# a.push GetComputerStatus.new

a.push InterpretTelemetry.new
#a.push InterpretHKTelemetry.new

a.push IoContextManager.new

a.push ComMosquittoManager.new

a.push TelemMosquittoManager.new

a.push PassTelemetry.new

#a.push PushToMySQL.new

a.push ReceiveTelemetry.new

a.push RunIDManager.new

a.push SendCommandToDAQComputer.new

a.push SendTelemetry.new

#a.push ShutdownSystem.new

a.push SocketCommunicationManager.new

#a.push SPIManager.new

### make documentation
begin
  a.define
rescue RuntimeError => e
  puts e
  nil
ensure
  a.make_doc(output: "../html/module_detail.xml", namespace: "GRAMSBalloon")
end
