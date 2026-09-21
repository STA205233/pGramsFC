#ifndef GB_BayCatI2CIO_hh
#define GB_BayCatI2CIO_hh 1
#include "BayCatAPICaller.hh"
#include "I2CInterface.hh"
#include <vector>
namespace gramsballoon::pgrams {
/**
 * @brief A class to control the I2C interface of BayCat via the VersaLogic API
 * @author Shota Arai
 * @date 2026-**-** | Shota Arai | First version
 */
class BayCatI2CIO: public I2CInterface, public BayCatAPICaller {
public:
  BayCatI2CIO() = default;
  virtual ~BayCatI2CIO() = default;
  BayCatI2CIO(const BayCatI2CIO &) = delete;

public:
  /**
   * @brief Apply the current setting, i.e. the clock frequency, to the I2C bus
   */
  int applySetting() override;
  /**
   * @brief Open the I2C bus
   * @param[in] channel Channel number to be opened
   */
  int Open(int channel) override;
  /**
   * @brief Close the I2C bus
   */
  int Close() override;
  /**
   * @brief Read a register of an I2C device
   * @param[in] address Address of the target device
   * @param[in] reg_data Register number to be read
   * @param[out] read_data Buffer to store the read data
   * @param[in] read_length Number of the bytes to be read
   */
  int ReadRegister(uint16_t address, uint8_t reg_data, uint8_t *read_data, uint32_t read_length) override;
  /**
   * @brief Write data to an I2C device
   * @param[in] address Address of the target device
   * @param[in] writeBuffer Data to be written
   * @param[in] size Number of the bytes to be written
   */
  int Write(uint16_t address, const uint8_t *writeBuffer, uint32_t size) override;
  /**
   * @brief Read data from an I2C device
   * @param[in] address Address of the target device
   * @param[out] readBuffer Buffer to store the read data
   * @param[in] size Number of the bytes to be read
   */
  int Read(uint16_t address, uint8_t *readBuffer, uint32_t size) override;

private:
  std::vector<uint8_t> buffer_;
};
} // namespace gramsballoon::pgrams
#endif //GB_BayCatI2CIO_hh