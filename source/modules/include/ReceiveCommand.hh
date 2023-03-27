/**
 * Receive commands from ground.
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 */


#ifndef ReceiveCommand_H
#define ReceiveCommand_H 1

#include <anlnext/BasicModule.hh>
#include <queue>
#include "CommandInterpreter.hh"
#include "ReadWaveform.hh"
#include "SerialCommunication.hh"

class ReadWaveform;

namespace GRAMSBalloon {

class ReceiveCommand : public anlnext::BasicModule
{
  DEFINE_ANL_MODULE(ReceiveCommand, 1.0);

public:
  ReceiveCommand();
  virtual ~ReceiveCommand();
  
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  void applyCommand();

private:
  std::vector<uint8_t> command_;
  std::queue<uint8_t> que_;
  std::unique_ptr<CommandInterpreter> comintp_ = nullptr;
  ReadWaveform* readWaveform_ = nullptr;
  std::string readWaveformModuleName_ = "ReadWaveform";

  //communication
  std::unique_ptr<SerialCommunication> sc_ = nullptr;
  speed_t baudrate_;
  std::string serialPath_;
  char openMode_ = O_RDWR;
  bool startReading_ = false;

};

} /* namespace GRAMSBalloon */

#endif /* ReceiveCommand_H */
