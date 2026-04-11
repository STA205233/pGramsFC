#ifndef GB_DEMO_MODE
#include "BME680IO.hh"
#include <iomanip>
#include <vector>

namespace gramsballoon::pgrams {

BME680IO::BME680IO() {
  bme68xn_ = std::make_unique<bme68x_dev>();
  sensorData_ = std::make_unique<bme68x_data>();
  configure_ = std::make_unique<bme68x_conf>();
}

int8_t BME680IO::readRegSPI(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  int8_t rslt = BME68X_OK;
  SPIInterface *intf = static_cast<SPIInterface *>(intf_ptr);
  const int result = intf->WriteThenRead(&reg_addr, BME_REGISTER_BYTES, reg_data, static_cast<uint32_t>(length));
  if (result < 0) {
    rslt = -1;
  }
  return rslt;
}

int8_t BME680IO::writeRegSPI(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  int8_t rslt = BME68X_OK;
  SPIInterface *intf = static_cast<SPIInterface *>(intf_ptr);
  std::vector<uint8_t> writeBuffer(length + 1);
  writeBuffer[0] = reg_addr;
  std::copy(reg_data, reg_data + length, writeBuffer.begin() + 1);
  const int result = intf->Write(writeBuffer.data(), static_cast<unsigned int>(writeBuffer.size()));
  if (result < 0) {
    rslt = -1;
  }
  return rslt;
}
int8_t BME680IO::writeRegI2C(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  int8_t rslt = BME68X_OK;
  I2CInterface *intf = static_cast<I2CInterface *>(intf_ptr);
  std::vector<uint8_t> writeBuffer(length + 1);
  writeBuffer[0] = reg_addr;
  std::copy(reg_data, reg_data + length, writeBuffer.begin() + 1);
  const int result = intf->Write(writeBuffer.data(), static_cast<unsigned int>(writeBuffer.size()));
  if (result < 0) {
    rslt = -1;
  }
  return rslt;
}
int8_t BME680IO::readRegI2C(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  int8_t rslt = BME68X_OK;
  I2CInterface *intf = static_cast<I2CInterface *>(intf_ptr);
  const int result = intf->WriteThenRead(&reg_addr, BME_REGISTER_BYTES, reg_data, static_cast<uint32_t>(length));
  if (result < 0) {
    rslt = -1;
  }
  return rslt;
}

void BME680IO::delay(uint32_t period, void *) {
  std::this_thread::sleep_for(std::chrono::microseconds(period));
}
void BME680IO::setupImpl() {
  sensorData_->pressure = 0;
  sensorData_->humidity = 0;
  sensorData_->temperature = 0;

  configure_->os_hum = BME68X_OS_1X;
  configure_->os_pres = BME68X_OS_1X;
  configure_->os_temp = BME68X_OS_1X;
  configure_->filter = BME68X_FILTER_OFF;

  bme68x_init(bme68xn_.get());
  bme68x_set_conf(configure_.get(), bme68xn_.get());
}
void BME680IO::setup(SPIInterface *intf) {

  bme68xn_->intf = bme68x_intf::BME68X_SPI_INTF;
  bme68xn_->read = BME680IO::readRegSPI;
  bme68xn_->write = BME680IO::writeRegSPI;
  bme68xn_->delay_us = BME680IO::delay;
  bme68xn_->intf_ptr = intf;

  setupImpl();
}
void BME680IO::setup(I2CInterface *intf) {
  bme68xn_->intf = bme68x_intf::BME68X_I2C_INTF;
  bme68xn_->read = BME680IO::readRegI2C;
  bme68xn_->write = BME680IO::writeRegI2C;
  bme68xn_->delay_us = BME680IO::delay;
  bme68xn_->intf_ptr = intf;

  setupImpl();
}

int BME680IO::getData() {
  bme68x_init(bme68xn_.get());
  bme68x_set_conf(configure_.get(), bme68xn_.get());
  bme68x_set_op_mode(BME68X_FORCED_MODE, bme68xn_.get());
  uint8_t ndata = 0;
  int res = bme68x_get_data(BME68X_FORCED_MODE, sensorData_.get(), &ndata, bme68xn_.get());

  if (res != 0) {
    std::cerr << "BME680IO::getData failed: ndata = " << static_cast<int>(ndata) << ", get_data_id = " << res << std::endl;
  }
  bme68x_set_op_mode(BME68X_SLEEP_MODE, bme68xn_.get());

  return res;
}

void BME680IO::printData() {
  std::cout << "Pressure: " << sensorData_->pressure << "\nHumidity: " << sensorData_->humidity << "\nTemperature: " << sensorData_->temperature << std::endl;
}

} // namespace gramsballoon::pgrams
#endif /* GB_DEMO_MODE */
