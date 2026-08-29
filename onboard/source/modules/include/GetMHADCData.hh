#ifndef GRAMSBalloon_GetMHADCData_hh
#define GRAMSBalloon_GetMHADCData_hh 1
#include "VHousekeepingModule.hh"
#include <regex>

namespace gramsballoon::pgrams {
class SendTelemetry;
class EncodedSerialCommunicator;

/**
 *  Module for communicating with MHADC.
 *
 * @author Shota Arai
 * @date 2024-09-19
 * @date 2026-08-28  | Use of VHousekeepingModule
 */
class GetMHADCData: public VHousekeepingModule {
  DEFINE_ANL_MODULE(GetMHADCData, 1.1);
  ENABLE_PARALLEL_RUN();

public:
  GetMHADCData() = default;
  virtual ~GetMHADCData() = default;

protected:
  GetMHADCData(const GetMHADCData &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_finalize() override;
  const std::vector<int> &AdcData() const {
    return singleton_self()->adcData_;
  }
  int NumCH() const {
    return singleton_self()->numCh_;
  }

private:
  int numCh_ = 32;
  int numSection_ = 6;
  int channelPerSection_ = 8;
  SendTelemetry *sendTelemetry_ = nullptr;
  std::vector<int> adcData_;
  std::vector<std::regex> regs_;
  int sleepForMsec_ = 500;
  int chatter_ = 0;
  int numTrials_ = 10;
  std::string dat_;
  EncodedSerialCommunicator *encodedSerialCommunicator_ = nullptr;
  std::string encodedSerialCommunicatorName_ = "EncodedSerialCommunicator";
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_GetMHADCData_hh