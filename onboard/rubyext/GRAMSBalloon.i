%module GRAMSBalloon
%{
// include headers of my modules
#include <anlnext/BasicModule.hh>
#include <string>
#include "SimpleLoop.hh"
#include "Sleep.hh"
#ifndef GB_MAC
#include "ReceiveCommand.hh"
#endif
#ifndef GB_MAC
#include "SendTelemetry.hh"
#endif
#ifdef USE_SPI
#include "SPIManager.hh"
#endif
#ifdef USE_SYSTEM_MODULES
#include "GetComputerStatus.hh"
#include "ShutdownSystem.hh"
#endif
#include "ReceiveTelemetry.hh"
#include "InterpretTelemetry.hh"
#ifdef USE_HSQUICKLOOK
#include "PushToMongoDB.hh"
#endif
#include "RunIDManager.hh"
#include "ReadTelemetry.hh"
#include "DumpSerial.hh"
#include "GetMHADCData.hh"
#include "EncodedSerialCommunicator.hh"

#ifdef USE_MYSQL
#include "PushToMySQL.hh"
#include "TreatToFCallback.hh"
#endif
#include "MosquittoManager.hh"
#include "SendArrayByMQTT.hh"
#include "DistributeCommand.hh"
#include "ReceiveStatusFromDAQComputer.hh"
#include "DividePacket.hh"
#include "PassTelemetry.hh"
#include "IoContextManager.hh"
#include "SendCommandToDAQComputer.hh"
#include "DetectErrorCallbackFromDAQ.hh"
#include "PressureGaugeManager.hh"
#include "GetPressure.hh"
%}

%include "std_vector.i"
%include "stdint.i"
%include "std_string.i"
%import(module="anlnext/ANL") "anlnext/ruby/ANL.i"

// interface to my modules

namespace gramsballoon {

class SimpleLoop : public anlnext::BasicModule
{
public:
  SimpleLoop();
};
namespace pgrams{
class Sleep : public anlnext::BasicModule
{
public:
  Sleep();
};
#ifdef USE_SPI
class SPIManager : public anlnext::BasicModule
{
public:
  SPIManager();
};
#endif

#ifdef USE_SYSTEM_MODULES
class GetComputerStatus : public anlnext::BasicModule
{
public:
  GetComputerStatus();
};
#endif
class ReceiveCommand : public anlnext::BasicModule
{
public:
  ReceiveCommand();
};
class SendTelemetry : public anlnext::BasicModule
{
public:
  SendTelemetry();
};
class ReceiveTelemetry :  public anlnext::BasicModule
{
public:
  ReceiveTelemetry();
};

//class DumpSerial: public anlnext::BasicModule {
//public:
//    DumpSerial();
//};

class GetMHADCData: public anlnext::BasicModule {
public:
  GetMHADCData();
};

class EncodedSerialCommunicator: public anlnext::BasicModule {
public:
  EncodedSerialCommunicator();
};

class IoContextManager: public anlnext::BasicModule{
public:
  IoContextManager();
};
class ReceiveStatusFromDAQComputer: public anlnext::BasicModule{
public:
  ReceiveStatusFromDAQComputer();
};
class SocketCommunicationManager: public anlnext::BasicModule{
public:
  SocketCommunicationManager();
};
class SendCommandToDAQComputer: public anlnext::BasicModule {
public:
  SendCommandToDAQComputer();
};
#ifdef USE_MYSQL
class PushToMySQL : public anlnext::BasicModule
{
public:
  PushToMySQL();
};
class TreatToFCallback: public anlnext::BasicModule {
public:
  TreatToFCallback();
};
#endif
template <typename TelemType>
class MosquittoManager: public anlnext::BasicModule
{
public:
  MosquittoManager();
};
class DistributeCommand: public anlnext::BasicModule {
public:
  DistributeCommand();
};
class DividePacket: public anlnext::BasicModule {
public:
  DividePacket();
};
class PassTelemetry: public anlnext::BasicModule {
public:
  PassTelemetry();
};
class InterpretTelemetry : public anlnext::BasicModule
{
public:
  InterpretTelemetry();
};
#ifdef USE_SYSTEM_MODULES
class ShutdownSystem : public anlnext::BasicModule
{
public:
  ShutdownSystem();
};
#endif
class RunIDManager : public anlnext::BasicModule
{
public:
  RunIDManager();
};

class SendArrayByMQTT : public anlnext::BasicModule
{
public:
  SendArrayByMQTT();
};

} // namespace pgrams



#ifdef USE_HSQUICKLOOK
class PushToMongoDB : public anlnext::BasicModule
{
public:
  PushToMongoDB();
};
#endif


namespace pgrams{
class MeasureOrientationByMHADC: public anlnext::BasicModule {
public:
  MeasureOrientationByMHADC();
};

class DetectErrorCallbackFromDAQ: public anlnext::BasicModule {
public:
  DetectErrorCallbackFromDAQ();
};

class PressureGaugeManager: public anlnext::BasicModule
{
public:
  PressureGaugeManager();
};

class GetPressure: public anlnext::BasicModule {
public:
  GetPressure();
};
} // namespace pgrams
} // namespace GRAMSBalloon
%template(TelemMosquittoManager) gramsballoon::pgrams::MosquittoManager<std::string>;
%template(ComMosquittoManager) gramsballoon::pgrams::MosquittoManager<std::vector<uint8_t>>;