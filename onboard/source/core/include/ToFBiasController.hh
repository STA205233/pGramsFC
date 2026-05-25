#ifndef GB_ToFBiasController_hh
#define GB_ToFBiasController_hh 1
#include "EncodedSerialCommunication.hh"
#include "HKDataSaver.hh"
#include <ostream>
namespace gramsballoon::pgrams {

class ToFBiasController: public EncodedSerialCommunication, public HKDataSaver<uint8_t> {
  static constexpr int ALL_CHANNELS = -1;
  static constexpr int NUM_ADC = 128;
  static constexpr int NUM_TEMP = 32;
  static constexpr int NUM_DAC = 128;
  static constexpr int NUM_DATA = NUM_ADC * 4 + NUM_TEMP * 4 + NUM_DAC * 3;

public:
  ToFBiasController();
  ToFBiasController(const std::string &serial_path);
  virtual ~ToFBiasController();
  int enableDataStream();
  int disableDataStream();
  int setTrimVoltage(int channel, int voltage);
  int setDefaultTemperature(int channel, int temperature);
  int enableTemperatureControl(int channel);
  int disableTemperatureControl(int channel);
  std::ostream &printData(std::ostream &os);
  int readData();
  void interpret(const char *dataStr);

protected:
  using EncodedSerialCommunication::ReadData;
  using EncodedSerialCommunication::ReadDataUntilBreak;
  using EncodedSerialCommunication::ReadDataUntilSpecificStr;
  using EncodedSerialCommunication::setBaudrate;
  using EncodedSerialCommunication::setOpenMode;
  using EncodedSerialCommunication::sread;
  using EncodedSerialCommunication::sreadSingle;
  using EncodedSerialCommunication::swrite;
  using EncodedSerialCommunication::WaitForTimeOut;

private:
  std::array<int, NUM_ADC> adcData_;
  char dataStr_[NUM_DATA];
  std::array<int, NUM_TEMP> tempData_;
  std::array<int, NUM_DAC> dacData_;
};

} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasController_hh