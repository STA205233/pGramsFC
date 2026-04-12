#ifndef GB_I2CManager_hh
#define GB_I2CManager_hh 1
#include "I2CInterface.hh"
#include "anlnext/BasicModule.hh"
namespace gramsballoon::pgrams {
/**
 * @brief A module to manage I2C communication.
 * @author Shota Arai
 * @date 2026-04-12 | Shota Arai | Created
 */
class I2CManager: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(I2CManager, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  I2CManager() = default;
  virtual ~I2CManager() = default;

protected:
  I2CManager(const I2CManager &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_finalize() override;

private:
  std::shared_ptr<I2CInterface> i2cInterface_ = nullptr;

public:
  std::shared_ptr<I2CInterface> &Interface() { return i2cInterface_; }
};
} // namespace gramsballoon::pgrams
#endif // GB_I2CManager_hh