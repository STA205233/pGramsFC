#ifndef BME280IO_H
#define BME280IO_H 1

#ifndef GB_DEMO_MODE

#include "I2CInterface.hh"
#include "SPIInterface.hh"
#include <cstdint>
#include <memory>

#define BME_REGISTER_BYTES 1
struct bme280_dev;
struct bme280_data;
struct bme280_settings;

namespace gramsballoon::pgrams {

/**
  * @brief A class to handle BME280
  *
  * @author Shota Arai
  * @date 2026-05-08 | Shota Arai | First design
  * @ingroup device_core
  */
class BME280IO {
public:
  BME280IO();
  /**
   * @brief Read registers via SPI. Registered as a read callback of the Bosch BME280 driver
   * @param[in] reg_addr Address of the first register to be read
   * @param[out] reg_data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @param[in] intf_ptr Pointer to the SPIInterface used for the communication
   */
  static int8_t readRegSPI(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
  /**
   * @brief Read registers via I2C. Registered as a read callback of the Bosch BME280 driver
   * @param[in] reg_addr Address of the first register to be read
   * @param[out] reg_data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @param[in] intf_ptr Pointer to the I2CInterface used for the communication
   */
  static int8_t readRegI2C(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
  /**
   * @brief Write registers via SPI. Registered as a write callback of the Bosch BME280 driver
   * @param[in] reg_addr Address of the first register to be written
   * @param[in] reg_data Data to be written
   * @param[in] length Number of the bytes to be written
   * @param[in] intf_ptr Pointer to the SPIInterface used for the communication
   */
  static int8_t writeRegSPI(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
  /**
   * @brief Write registers via I2C. Registered as a write callback of the Bosch BME280 driver
   * @param[in] reg_addr Address of the first register to be written
   * @param[in] reg_data Data to be written
   * @param[in] length Number of the bytes to be written
   * @param[in] intf_ptr Pointer to the I2CInterface used for the communication
   */
  static int8_t writeRegI2C(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
  /**
   * @brief Sleep for the given period. Registered as a delay callback of the Bosch BME280 driver
   * @param[in] period Sleep period in microseconds
   * @param[in] intf_ptr Pointer to the interface, not used
   */
  static void delay(uint32_t period, void *intf_ptr);
  /**
   * @brief Set up the sensor to be accessed via I2C
   * @param[in] i2cInterface Pointer to the I2C interface
   */
  void setup(I2CInterface *i2cInterface);
  /**
   * @brief Set up the sensor to be accessed via SPI
   * @param[in] spiInterface Pointer to the SPI interface
   */
  void setup(SPIInterface *spiInterface);
  /**
   * @brief Measure and retrieve the sensor data, which can be accessed via SensorData()
   */
  int getData();
  /**
   * @brief Print the latest sensor data to the standard output
   */
  void printData();

  /**
   * @brief Return the pointer to the latest sensor data
   */
  bme280_data *SensorData() { return sensorData_.get(); }

private:
  std::unique_ptr<bme280_dev> bme280n_ = nullptr;
  std::unique_ptr<bme280_data> sensorData_ = nullptr;
  std::unique_ptr<bme280_settings> configure_ = nullptr;
  void setupImpl();

  void appendUnits();
};

} // namespace gramsballoon::pgrams

#endif
#endif /* BME280IO_H */
