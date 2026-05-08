#include "I2CManager.hh"
#include "BayCatI2CIO.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus I2CManager::mod_define() {
  return AS_OK;
}
ANLStatus I2CManager::mod_initialize() {
#ifdef USE_BAYCAT
  i2cInterface_ = std::make_shared<BayCatI2CIO>();
#else
  std::cerr << "BayCat I2C control type is not supported in this build." << std::endl;
  return AS_QUIT_ALL_ERROR;
#endif
  i2cInterface_->Open(0);
  return AS_OK;
}
ANLStatus I2CManager::mod_finalize() {
  if (i2cInterface_ && i2cInterface_->IsOpen()) {
    i2cInterface_->Close();
  }
  i2cInterface_.reset();
  return AS_OK;
}
} // namespace gramsballoon::pgrams