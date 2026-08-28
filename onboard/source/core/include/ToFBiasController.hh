#ifndef GB_ToFBiasController_hh
#define GB_ToFBiasController_hh 1
#include "EncodedSerialCommunication.hh"
#include "HKDataSaver.hh"
#include <ostream>
#include <string>
#include <string_view>
namespace gramsballoon::pgrams {

/**
 * @brief Control class for ToF Bias
 * @author Shota Arai
 * @date 2026-07-06 | Shota Arai | First version
 */
class ToFBiasController final: public EncodedSerialCommunication, public HKDataSaver<std::string> {
  // for telemetry
  static constexpr int NUM_DATA = 5000;
  // for command
  static constexpr int NUM_DCDC_SETTING = 8;
  static constexpr int NUM_TEMP_SETTING = 16;

public:
  ToFBiasController();
  ToFBiasController(const std::string &serial_path);
  virtual ~ToFBiasController();
  int enableDataStream();
  int disableDataStream();
  int queryFullOutput();
  int getFullOutput();
  int getOnePacket(std::string &str);
  int enableDCDC(int channel);
  int disableDCDC(int channel);
  int setVoffset(int voltage);
  int setTmuxChannel(int channel, int on_off);
  int setVdef(int channel, int voltage);
  std::ostream &printData(std::ostream &os);

protected:
  using EncodedSerialCommunication::ReadDataUntilBreak;
  using EncodedSerialCommunication::ReadDataUntilSpecificStr;
  using EncodedSerialCommunication::setBaudrate;
  using EncodedSerialCommunication::setOpenMode;
  int sendCommand(std::string_view data);

private:
  std::string dataStr_;
  bool isFullOutputQueried_ = false;
};

} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasController_hh