#ifndef BME280IO_H
#define BME280IO_H 1

#ifndef GB_DEMO_MODE

#include "I2CInterface.hh"
#include "SPIInterface.hh"
#include "bme280.h"
#include "bme280_defs.h"
#include <chrono>
#include <iostream>
#include <thread>

#define BME_REGISTER_BYTES 1

/**
 * A class to handle BME280
 *
 * @author Shota Arai
 * @date 2026-05-08 | Shota Arai | First design
 */

namespace gramsballoon::pgrams {

class BME280IO {
public:
  BME280IO();
  static int8_t readRegSPI(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
  static int8_t readRegI2C(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
  static int8_t writeRegSPI(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
  static int8_t writeRegI2C(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
  static void delay(uint32_t period, void *intf_ptr);
  void setup(I2CInterface *i2cInterface);
  void setup(SPIInterface *spiInterface);
  int getData();
  void printData();

  bme280_data *SensorData() { return sensorData_.get(); }

private:
  std::unique_ptr<bme280_dev> bme280n_ = nullptr;
  std::unique_ptr<bme280_data> sensorData_ = nullptr;
  std::unique_ptr<bme280_settings> configure_ = nullptr;
  void setupImpl();
};

} // namespace gramsballoon::pgrams

#endif
#endif /* BME280IO_H */
