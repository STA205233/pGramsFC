#include "BME280IO.hh"
#include <iomanip>
#include <vector>

namespace gramsballoon::pgrams {
constexpr uint8_t BME_ADDRESS = 0x77;

BME280IO::BME280IO() {
  bme280n_ = std::make_unique<bme280_dev>();
  sensorData_ = std::make_unique<bme280_data>();
  configure_ = std::make_unique<bme280_settings>();
}

int8_t BME280IO::readRegSPI(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  int8_t rslt = BME280_OK;
  SPIInterface *intf = static_cast<SPIInterface *>(intf_ptr);
  const int result = intf->WriteThenRead(&reg_addr, BME_REGISTER_BYTES, reg_data, static_cast<uint32_t>(length));
  if (result < 0) {
    rslt = -1;
  }
  return rslt;
}

int8_t BME280IO::writeRegSPI(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  int8_t rslt = BME280_OK;
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
int8_t BME280IO::writeRegI2C(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  int8_t rslt = BME280_OK;
  I2CInterface *intf = static_cast<I2CInterface *>(intf_ptr);
  intf->setAddress(BME_ADDRESS);
  std::vector<uint8_t> writeBuffer(length + 1);
  writeBuffer[0] = reg_addr;
  std::copy(reg_data, reg_data + length, writeBuffer.begin() + 1);
  const int result = intf->Write(writeBuffer.data(), static_cast<unsigned int>(writeBuffer.size()));
  if (result < 0) {
    rslt = -1;
  }
  return rslt;
}
int8_t BME280IO::readRegI2C(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  int8_t rslt = BME280_OK;
  I2CInterface *intf = static_cast<I2CInterface *>(intf_ptr);
  intf->setAddress(BME_ADDRESS);
  const int result = intf->WriteThenRead(&reg_addr, BME_REGISTER_BYTES, reg_data, static_cast<uint32_t>(length));
  if (result < 0) {
    rslt = -1;
  }
  return rslt;
}

void BME280IO::delay(uint32_t period, void *) {
  std::this_thread::sleep_for(std::chrono::microseconds(period));
}
void BME280IO::setupImpl() {
  sensorData_->pressure = 0;
  sensorData_->humidity = 0;
  sensorData_->temperature = 0;

  configure_->osr_p = BME280_OVERSAMPLING_1X;
  configure_->osr_h = BME280_OVERSAMPLING_1X;
  configure_->osr_t = BME280_OVERSAMPLING_1X;
  configure_->filter = BME280_FILTER_COEFF_OFF;

  bme280_init(bme280n_.get());
  bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, configure_.get(), bme280n_.get());
}
void BME280IO::setup(SPIInterface *intf) {

  bme280n_->intf = bme280_intf::BME280_SPI_INTF;
  bme280n_->read = BME280IO::readRegSPI;
  bme280n_->write = BME280IO::writeRegSPI;
  bme280n_->delay_us = BME280IO::delay;
  bme280n_->intf_ptr = intf;

  setupImpl();
}
void BME280IO::setup(I2CInterface *intf) {
  bme280n_->intf = bme280_intf::BME280_I2C_INTF;
  bme280n_->read = BME280IO::readRegI2C;
  bme280n_->write = BME280IO::writeRegI2C;
  bme280n_->delay_us = BME280IO::delay;
  bme280n_->intf_ptr = intf;

  setupImpl();
}

int BME280IO::getData() {
  bme280_init(bme280n_.get());
  bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, configure_.get(), bme280n_.get());
  bme280_set_sensor_mode(BME280_POWERMODE_FORCED, bme280n_.get());
  const int res = bme280_get_sensor_data(BME280_ALL, sensorData_.get(), bme280n_.get());

  if (res != 0) {
    std::cerr << "BME280IO::getData failed: get_data_id = " << res << std::endl;
  }
  bme280_set_sensor_mode(BME280_POWERMODE_SLEEP, bme280n_.get());

  return res;
}

void BME280IO::printData() {
  std::cout << "Pressure: " << sensorData_->pressure << "\nHumidity: " << sensorData_->humidity << "\nTemperature: " << sensorData_->temperature << std::endl;
}

} // namespace gramsballoon::pgrams
