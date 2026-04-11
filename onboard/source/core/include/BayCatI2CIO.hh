#ifndef GB_BayCatI2CIO_hh
#define GB_BayCatI2CIO_hh 1
#include "BayCatAPICaller.hh"
#include "I2CInterface.hh"
namespace gramsballoon::pgrams {
class BayCatI2CIO: public I2CInterface, public BayCatAPICaller {
public:
  BayCatI2CIO() = default;
  virtual ~BayCatI2CIO() = default;
  BayCatI2CIO(const BayCatI2CIO &) = delete;

private:
  int applySetting() override;
  int Open(int channel) override;
  int Close() override;
  int WriteThenRead(uint16_t address, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) override;
  int Write(uint16_t address, const uint8_t *writeBuffer, unsigned int size) override;
  int Read(uint16_t address, uint8_t *readBuffer, unsigned int size) override;
};
} // namespace gramsballoon::pgrams
#endif //GB_BayCatI2CIO_hh