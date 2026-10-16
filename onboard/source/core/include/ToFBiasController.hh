#ifndef GB_ToFBiasController_hh
#define GB_ToFBiasController_hh 1
#include "EncodedSerialCommunication.hh"
#include "HKDataSaver.hh"
#include <chrono>
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
  static constexpr int NUM_DATA = 1024;
  static constexpr int NUM_FULL_DATA = 5 * 1024 * 8;
  // for command
  static constexpr int NUM_DCDC_SETTING = 8;
  static constexpr int NUM_TEMP_SETTING = 16;
  static constexpr int NUM_CMD_DATA = 64;

public:
  enum class TofBiasMode {
    DEBUG,
    AUTO
  };

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
  int setMode(TofBiasMode mode);
  const std::string &getData() const { return fullOutputStr_; }
  int refresh();

  static constexpr std::string_view Mode2Str(TofBiasMode mode);

protected:
  using EncodedSerialCommunication::ReadDataUntilBreak;
  using EncodedSerialCommunication::ReadDataUntilSpecificStr;
  using EncodedSerialCommunication::setBaudrate;
  using EncodedSerialCommunication::setOpenMode;
  int sendCommand(std::string_view data);

private:
  void appendInt(int value);

private:
  std::string dataStr_;
  bool isFullOutputQueried_ = false;
  std::chrono::microseconds firstTimeout_;
  std::string fullOutputStr_;
  std::string cmdStr_;
};

constexpr std::string_view ToFBiasController::Mode2Str(ToFBiasController::TofBiasMode mode) {
  return mode == TofBiasMode::DEBUG ? "debug" : "auto";
}

} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasController_hh