#ifndef ReadTelemetry_H
#define ReadTelemetry_H 1
#if 0

#include "ReceiveTelemetry.hh"
#include <anlnext/BasicModule.hh>
#include <fstream>

namespace gramsballoon {

class ReadTelemetry: public pgrams::ReceiveTelemetry {
  DEFINE_ANL_MODULE(ReadTelemetry, 1.0);

public:
  ReadTelemetry();
  virtual ~ReadTelemetry();

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  uint8_t readOneByte();

private:
  std::vector<std::string> filenames_;
  int fileIndex_ = 0;
  int sleepms_ = 500;
  std::ifstream ifs_;
  bool done_ = false;
};

} // namespace gramsballoon
#endif // This module is currently disabled
#endif // ReadTelemetry_H