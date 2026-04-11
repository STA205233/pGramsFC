#ifndef BME680IO_H
#define BME680IO_H 1

#ifndef GB_DEMO_MODE

#include "I2CInterface.hh"
#include "SPIInterface.hh"
#include "bme68x.h"
#include "bme68x_defs.h"
#include <chrono>
#include <iostream>
#include <thread>

#define BME_REGISTER_BYTES 1

/**
 * A class to handle BME680
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 */

namespace gramsballoon::pgrams {

class BME680IO {
public:
  BME680IO();
  static int8_t readRegSPI(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
  static int8_t readRegI2C(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
  static int8_t writeRegSPI(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
  static int8_t writeRegI2C(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
  static void delay(uint32_t period, void *intf_ptr);
  void setup(I2CInterface *i2cInterface);
  void setupImpl();
  void setup(SPIInterface *spiInterface);
  int getData();
  void printData();

  bme68x_data *SensorData() { return sensorData_.get(); }

private:
  std::unique_ptr<bme68x_dev> bme68xn_ = nullptr;
  std::unique_ptr<bme68x_data> sensorData_ = nullptr;
  std::unique_ptr<bme68x_conf> configure_ = nullptr;
};

} // namespace gramsballoon::pgrams

#endif
#endif /* BME680IO_H */
