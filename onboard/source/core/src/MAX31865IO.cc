#include "MAX31865IO.hh"
#include <bitset>
#include <iomanip>
#include <vector>

namespace gramsballoon::pgrams {

MAX31865IO::MAX31865IO() {
}

void MAX31865IO::setInterface(SPIInterface *intf) {
  intf_ = intf;
}

int MAX31865IO::readReg(uint8_t reg, uint8_t *value, unsigned int length) {
  if (intf_ == nullptr) {
    std::cerr << "intf_ is null." << std::endl;
    return MAX31865_BAD;
  }
  reg &= ~MAX31865_WRITE_MSK;
  FT_STATUS status = intf_->WriteAfterRead(cs_, &reg, 1, value, length);
  if (status != FT_OK) {
    return status;
  }

  std::cout << "MAX31865IO::readReg() reg: " << std::hex << static_cast<int>(reg) << " value: ";
  for (unsigned int i = 0; i < length; ++i) {
    std::cout << std::hex << static_cast<int>(value[i]) << " ";
  }
  std::cout << std::endl;
  return length;
}

int MAX31865IO::writeReg(uint8_t reg, char *value, unsigned int length) {
  if (intf_ == nullptr) {
    std::cerr << "intf_ is null." << std::endl;
    return MAX31865_BAD;
  }
  reg |= MAX31865_WRITE_MSK;
  int rslt = MAX31865_OK;
  writeBuffer_.clear();
  writeBuffer_.push_back(reg);
  for (unsigned int i = 0; i < length; ++i) {
    writeBuffer_.push_back(static_cast<uint8_t>(value[i]));
  }
  FT_STATUS status = intf_->Write(cs_, &writeBuffer_[0], 1 + length);
  if (status != FT_OK) {
    std::cerr << "write_reg failed. status = " << status << std::endl;
    rslt = MAX31865_BAD;
    return rslt;
  }
  return length;
}

int MAX31865IO::setConfigure(char bits) {
  int bias_ = getBias();

  if ((bits & MAX31865_CONF_CONVERSION_MSK) == MAX31865_CONF_CONVERSION_AUTO) {
    bits = (bits & (~MAX31865_CONF_FILTER_SEL_MSK)) | (getFilter() & MAX31865_CONF_FILTER_SEL_MSK);
  }
  int dbg = writeReg(MAX31865_CONF_WRITE_REG, &bits, 1);
  if (dbg != 1) {
    std::cerr << "set_configure failed due to write_reg failure. write_reg_bytes: " << dbg << std::endl;
  }

  if ((bias_ != getBias()) && (bias_ == MAX31865_CONF_BIAS_OFF)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return MAX31865_OK;
}

int MAX31865IO::setConfigureSingle(char val, char mask) {
  char bits = (currentBits_ & ~mask) | (val & mask);
  int status = setConfigure(bits);
  if ((currentBits_ & mask) != (val & mask)) {
    return MAX31865_BAD;
  }
  return status;
}

int MAX31865IO::updateConfigure() {
  int rslt = readReg(MAX31865_CONF_READ_REG, &currentBits_, 1);
  if (rslt != 1) {
    std::cerr << "get_configure failed to read. read bytes: " << rslt << std::endl;
    return MAX31865_BAD;
  }
  return MAX31865_OK;
}

int16_t MAX31865IO::getData() {
  int16_t data = 0;
  uint8_t temp[2];
  readReg(MAX31865_DATA_REG_MSB, &temp[0], 1);
  readReg(MAX31865_DATA_REG_LSB, &temp[1], 1);

  if ((temp[1] & MAX31865_DATA_FAULT_MSK) == 0) {
    data |= temp[0] << 7;
    data |= temp[1] >> 1;
  }
  else {
    data = 0;
    std::cout << "MAX31865IO::getData() deteted fault." << std::endl;
    getFaultStatus();
    faultStatusClear();
    return MAX31865_BAD;
  }
  temperatureADC_ = data;
  temperature_ = Tconversion(data);
  return MAX31865_OK;
}

int MAX31865IO::faultStatusClear() {
  if (updateConfigure() < 0) {
    std::cerr << "fault_status_clear may change configure due to get_configure failure." << std::endl;
    return MAX31865_WARNING;
  }
  char bits = getConfigure();
  bits &= ~MAX31865_CONF_1SHOT_MSK;
  bits &= ~MAX31865_CONF_FD_CYCLE_MSK;
  bits |= MAX31865_CONF_F_STATUS_CLR_MSK;

  int rslt_write = writeReg(MAX31865_CONF_WRITE_REG, &bits, 1);
  updateConfigure();
  std::this_thread::sleep_for(std::chrono::microseconds(100));
  if (rslt_write != 1) {
    std::cerr << "fault_status_clear may not be executed. rslt_write = " << rslt_write << std::endl;
    return MAX31865_BAD;
  }
  return MAX31865_OK;
}

int MAX31865IO::getFaultStatus() {
  uint8_t bits = 0b0;
  int rslt_read = readReg(MAX31865_FAULT_STATUS_REG, &bits, 1);
  if (rslt_read != 1) {
    std::cerr << "get_fault_status failed due to read_reg failure. rslt_read = " << rslt_read << std::endl;
    return MAX31865_BAD;
  }
  std::cout << "MAX31865 Fault status : " << std::bitset<8>(bits) << std::endl;
  return MAX31865_OK;
}

double MAX31865IO::Tconversion(int adc) {
  double res = (static_cast<double>(adc) * static_cast<double>(rRef_) / 400.0) / 32.0 - 256.0;
  return res;
}

int MAX31865IO::setBias(char val) {
  return setConfigureSingle(val, MAX31865_CONF_BIAS_MSK);
}

int MAX31865IO::setConversion(char val) {
  return setConfigureSingle(val, MAX31865_CONF_CONVERSION_MSK);
}

int MAX31865IO::setWires(char val) {
  return setConfigureSingle(val, MAX31865_CONF_WIRE_MSK);
}

int MAX31865IO::setFaultDetection(char val) {
  return setConfigureSingle(val, (MAX31865_CONF_FD_CYCLE_MSK | MAX31865_CONF_F_STATUS_CLR_MSK));
}

int MAX31865IO::setFilter(char val) {
  return setConfigureSingle(val, MAX31865_CONF_FILTER_SEL_MSK);
}

char MAX31865IO::getBias() {
  updateConfigure();
  return (currentBits_ & MAX31865_CONF_BIAS_MSK);
}

char MAX31865IO::getConversion() {
  updateConfigure();
  return (currentBits_ & MAX31865_CONF_CONVERSION_MSK);
}

char MAX31865IO::getWires() {
  updateConfigure();
  return (currentBits_ & MAX31865_CONF_WIRE_MSK);
}

char MAX31865IO::getFaultDetection() {
  updateConfigure();
  return (currentBits_ & (MAX31865_CONF_FD_CYCLE_MSK | MAX31865_CONF_F_STATUS_CLR_MSK));
}

char MAX31865IO::getFilter() {
  updateConfigure();
  return (currentBits_ & MAX31865_CONF_FILTER_SEL_MSK);
}

} // namespace gramsballoon::pgrams
