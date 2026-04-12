#include "I2CManager.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {
ANLStatus I2CManager::mod_define() {
  return AS_OK;
}
ANLStatus I2CManager::mod_initialize() {
  i2cInterface_ = std::make_shared<I2CInterface>();
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