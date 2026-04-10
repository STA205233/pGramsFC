#include "BayCatAPICaller.hh"
#include <iostream>
namespace gramsballoon::pgrams {
BayCatAPICaller::BayCatAPICaller() {
  if (++counter_ == 0) {
    startAPI();
  }
}
BayCatAPICaller::~BayCatAPICaller() {
  if (--counter_ == 0) {
    stopAPI();
  }
}
BayCatAPICaller::BayCatAPICaller(const BayCatAPICaller &) {
  if (++counter_ == 0) {
    startAPI();
  }
}
int BayCatAPICaller::startAPI() {
  std::cout << "Initializing VersaLogic Library..." << std::endl;
  const auto ret = VL_Open();
  if (ret != 0) {
    std::cerr << "VL_Open failed: " << ret << std::endl;
    apiStatus_.store(ret);
  }
  else {
    apiStatus_.store(1);
  }
  return ret;
}
int BayCatAPICaller::stopAPI() {
  std::cout << "Closing VersaLogic Library..." << std::endl;
  const auto ret = VL_Close();
  apiStatus_.store(0);
  return ret;
}
} // namespace gramsballoon::pgrams
