#ifndef BME680IO_H
#define BME680IO_H 1

#ifndef GB_DEMO_MODE

#include "I2CInterface.hh"
#include "SPIInterface.hh"
#include <cstdint>
#include <memory>

#define BME_REGISTER_BYTES 1
struct bme68x_dev;
struct bme68x_data;
struct bme68x_conf;

namespace gramsballoon::pgrams {

/**
  * @brief A class to handle BME680
  *
  * @author Tsubasa Tamba, Shota Arai
  * @date 2023-03-01
  */
class BME680IO {
public:
  BME680IO();
  /**
   * @brief Read registers via SPI. Registered as a read callback of the Bosch BME68x driver
   * @param[in] reg_addr Address of the first register to be read
   * @param[out] reg_data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @param[in] intf_ptr Pointer to the SPIInterface used for the communication
   */
  static int8_t readRegSPI(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
  /**
   * @brief Read registers via I2C. Registered as a read callback of the Bosch BME68x driver
   * @param[in] reg_addr Address of the first register to be read
   * @param[out] reg_data Buffer to store the read data
   * @param[in] length Number of the bytes to be read
   * @param[in] intf_ptr Pointer to the I2CInterface used for the communication
   */
  static int8_t readRegI2C(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
  /**
   * @brief Write registers via SPI. Registered as a write callback of the Bosch BME68x driver
   * @param[in] reg_addr Address of the first register to be written
   * @param[in] reg_data Data to be written
   * @param[in] length Number of the bytes to be written
   * @param[in] intf_ptr Pointer to the SPIInterface used for the communication
   */
  static int8_t writeRegSPI(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
  /**
   * @brief Write registers via I2C. Registered as a write callback of the Bosch BME68x driver
   * @param[in] reg_addr Address of the first register to be written
   * @param[in] reg_data Data to be written
   * @param[in] length Number of the bytes to be written
   * @param[in] intf_ptr Pointer to the I2CInterface used for the communication
   */
  static int8_t writeRegI2C(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
  /**
   * @brief Sleep for the given period. Registered as a delay callback of the Bosch BME68x driver
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
  bme68x_data *SensorData() { return sensorData_.get(); }

private:
  std::unique_ptr<bme68x_dev> bme68xn_ = nullptr;
  std::unique_ptr<bme68x_data> sensorData_ = nullptr;
  std::unique_ptr<bme68x_conf> configure_ = nullptr;
  void setupImpl();

  void appendUnits();
};

} // namespace gramsballoon::pgrams

#endif
#endif /* BME680IO_H */
