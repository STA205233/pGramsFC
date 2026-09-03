%module GRAMSBalloon
%{
// include headers of my modules
#include <anlnext/BasicModule.hh>
#include <string>
#include "SimpleLoop.hh"
#include "Sleep.hh"
#include "ReceiveCommand.hh"
#include "SendTelemetry.hh"
#ifdef USE_SPI
#include "SPIManager.hh"
#include "GetPDUInfo.hh"
#include "ControlPDU.hh"
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
#ifdef USE_I2C
#include "GetEnvironmentalData.hh"
#include "I2CManager.hh"
#endif
#include "RunIDManager.hh"
#include "GetMHADCData.hh"
#include "EncodedSerialCommunicator.hh"

#ifdef USE_MYSQL
#include "PushToMySQL.hh"
#include "TreatToFCallback.hh"
#endif
#include "MosquittoManager.hh"
#include "SendArrayByMQTT.hh"
#include "SendPacketByMQTT.hh"
#include "DistributeCommand.hh"
#include "ReceiveStatusFromDAQComputer.hh"
#include "DividePacket.hh"
#include "PassTelemetry.hh"
#include "IoContextManager.hh"
#include "SendCommandToDAQComputer.hh"
#include "DetectErrorCallbackFromDAQ.hh"
#include "PressureGaugeManager.hh"
#include "GetPressure.hh"
#include "ControlToFBias.hh"
#ifdef USE_LJM
#include "GetLabJackData.hh"
#endif
#include "ConvertHubHKTelemetry.hh"
#include "VDBDataStore.hh"
#include "VHousekeepingModule.hh"
#include "SetHKEvs.hh"
#ifdef USE_MYSQL
#include "PushTBTelemetryToMySQL.hh"
#endif
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
  ~SimpleLoop();
};
namespace pgrams{
class Sleep : public anlnext::BasicModule
{
public:
  Sleep();
  ~Sleep();
};

class VHousekeepingModule: public anlnext::BasicModule {
public:
  VHousekeepingModule();
  ~VHousekeepingModule();
};
#ifdef USE_SPI
class SPIManager : public anlnext::BasicModule
{
public:
  SPIManager();
  ~SPIManager();
};
class GetPDUInfo : public VHousekeepingModule
{
public:
  GetPDUInfo();
  ~GetPDUInfo();
};
class ControlPDU : public anlnext::BasicModule
{
public:
  ControlPDU();
  ~ControlPDU();
};
#endif

#ifdef USE_SYSTEM_MODULES
class GetComputerStatus : public VHousekeepingModule
{
public:
  GetComputerStatus();
  ~GetComputerStatus();
};
#endif
class ReceiveCommand : public anlnext::BasicModule
{
public:
  ReceiveCommand();
  ~ReceiveCommand();
};
class SendTelemetry : public VHousekeepingModule
{
public:
  SendTelemetry();
  ~SendTelemetry();
};
class ReceiveTelemetry :  public anlnext::BasicModule
{
public:
  ReceiveTelemetry();
  ~ReceiveTelemetry();
};

class GetMHADCData: public VHousekeepingModule {
public:
  GetMHADCData();
  ~GetMHADCData();
};

class EncodedSerialCommunicator: public anlnext::BasicModule {
public:
  EncodedSerialCommunicator();
  ~EncodedSerialCommunicator();
};

class IoContextManager: public anlnext::BasicModule{
public:
  IoContextManager();
  ~IoContextManager();
};
class ReceiveStatusFromDAQComputer: public anlnext::BasicModule{
public:
  ReceiveStatusFromDAQComputer();
  ~ReceiveStatusFromDAQComputer();
};
class SocketCommunicationManager: public anlnext::BasicModule{
public:
  SocketCommunicationManager();
  ~SocketCommunicationManager();
};
class SendCommandToDAQComputer: public anlnext::BasicModule {
public:
  SendCommandToDAQComputer();
  ~SendCommandToDAQComputer();
};
#ifdef USE_MYSQL
class PushToMySQL : public anlnext::BasicModule
{
public:
  PushToMySQL();
  ~PushToMySQL();
};
class TreatToFCallback: public anlnext::BasicModule {
public:
  TreatToFCallback();
  ~TreatToFCallback();
};
class MySQLManager: public anlnext::BasicModule {
public:
  MySQLManager();
  ~MySQLManager();
};
#endif
template <typename TelemType>
class MosquittoManager: public anlnext::BasicModule
{
public:
  MosquittoManager();
  ~MosquittoManager();
};
class DistributeCommand: public anlnext::BasicModule {
public:
  DistributeCommand();
  ~DistributeCommand();
};
class DividePacket: public anlnext::BasicModule {
public:
  DividePacket();
  ~DividePacket();
};
class PassTelemetry: public anlnext::BasicModule {
public:
  PassTelemetry();
  ~PassTelemetry();
};
class InterpretTelemetry : public anlnext::BasicModule
{
public:
  InterpretTelemetry();
  ~InterpretTelemetry();
};
#ifdef USE_SYSTEM_MODULES
class ShutdownSystem : public anlnext::BasicModule
{
public:
  ShutdownSystem();
  ~ShutdownSystem();
};
#endif
class RunIDManager : public anlnext::BasicModule
{
public:
  RunIDManager();
  ~RunIDManager();
};

class SendArrayByMQTT : public anlnext::BasicModule
{
public:
  SendArrayByMQTT();
  ~SendArrayByMQTT();
};

class SendPacketByMQTT : public anlnext::BasicModule
{
public:
  SendPacketByMQTT();
  ~SendPacketByMQTT();
};

class DetectErrorCallbackFromDAQ: public anlnext::BasicModule {
public:
  DetectErrorCallbackFromDAQ();
  ~DetectErrorCallbackFromDAQ();
};

class PressureGaugeManager: public anlnext::BasicModule
{
public:
  PressureGaugeManager();
  ~PressureGaugeManager();
};

class GetPressure: public anlnext::BasicModule {
public:
  GetPressure();
  ~GetPressure();
};
class ControlToFBias: public anlnext::BasicModule {
public:
  ControlToFBias();
  ~ControlToFBias();
};
#ifdef USE_LJM
class GetLabJackData: public VHousekeepingModule {
public:
  GetLabJackData();
  ~GetLabJackData();
};
#endif
#ifdef USE_I2C
class GetEnvironmentalData: public VHousekeepingModule {
public:
  GetEnvironmentalData();
  ~GetEnvironmentalData();
};
class I2CManager: public anlnext::BasicModule {
public:
  I2CManager();
  ~I2CManager();
};
#endif
class ConvertHubHKTelemetry: public anlnext::BasicModule, public VDBDataStore {
public:
  ConvertHubHKTelemetry();
  ~ConvertHubHKTelemetry();
};

class SetHKEvs: public anlnext::BasicModule {
public:
  SetHKEvs();
  ~SetHKEvs();
};

#ifdef USE_MYSQL
class PushTBTelemetryToMySQL: public anlnext::BasicModule {
public:
  PushTBTelemetryToMySQL();
  ~PushTBTelemetryToMySQL();
};
#endif
} // namespace pgrams
} // namespace GRAMSBalloon
%template(TelemMosquittoManager) gramsballoon::pgrams::MosquittoManager<std::string>;
%template(ComMosquittoManager) gramsballoon::pgrams::MosquittoManager<std::vector<uint8_t>>;