#include "BayCatControl.hh"
namespace gramsballoon::pgrams {
BayCatControl::BayCatControl() {
  baudrateList_.emplace(750000, SPI_CLK_FREQ0);
  baudrateList_.emplace(1500000, SPI_CLK_FREQ1);
  baudrateList_.emplace(2000000, SPI_CLK_FREQ2);
  baudrateList_.emplace(6000000, SPI_CLK_FREQ3);
}
void BayCatControl::setBaudrate(unsigned int baudrate) {
  if (!baudrateList_.count(baudrate) == 0) {
    std::cerr << "Baudrate " << baudrate << " Hz is not working." << std::endl;
    return;
  }
  baudrate_ = baudrate;
  const VL_APIStatusT status = VSL_SPISetFrequency(baudrateList_[baudrate_]);
  if (status != VL_API_OK) {
    std::cerr << "SPISetFrequency failed: " << status << std::endl;
  }
}
void BayCatControl::setConfigOptions(unsigned int configOptions) {
  options_ = configOptions;
}
int BayCatControl::updateSetting() {
  bool failed = false;
  const VL_APIStatusT status = VSL_SPISetMode(options_ & SPI_MODE_MASK);
  if (status != VL_API_OK) {
    std::cerr << "SPISetMode failed: " << status << std::endl;
    failed = true;
  }
  const VL_APIStatusT status2 = VSL_SPISetShiftDirection(options_ & SPI_SHIFT_DIRECTION);
  if (status2 != VL_API_OK) {
    std::cerr << "SPISetShiftDiretcion failed: " << status << std::endl;
    failed = true;
  }
  if (failed) return -1;
  return 0;
}
int BayCatControl::Open(int) {
  if (isOpen_) {
    return 0;
  }
  const auto status = VSL_Open();
  return status;
}
int BayCatControl::Close() {
  if (!isOpen_) {
    return 0;
  }
  const auto status = VSL_Close();
  return status;
}
int BayCatControl::controlGPIO(int cs, bool value) {
  const auto status = VSL_DIOSetChannelLevel(cs, value);
  if (status != VL_API_OK) {
    std::cerr << "VSL_DIOSetChannelLevel failed: " << status << std::endl;
    return -1;
  }
  return 0;
}
} // namespace gramsballoon::pgrams