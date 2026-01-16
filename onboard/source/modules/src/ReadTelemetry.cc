#if 0 // This module is currently disabled
#include "ReadTelemetry.hh"
#include <chrono>
#include <thread>
using namespace anlnext;
using namespace gramsballoon::pgrams;
namespace gramsballoon {

ReadTelemetry::ReadTelemetry() {
}

ReadTelemetry::~ReadTelemetry() = default;

ANLStatus ReadTelemetry::mod_define() {
  ANLStatus status = ReceiveTelemetry::mod_define();
  if (status != AS_OK) {
    return status;
  }
  define_parameter("filenames", &mod_class::filenames_);
  define_parameter("sleep_ms", &mod_class::sleepms_);

  return AS_OK;
}

ANLStatus ReadTelemetry::mod_initialize() {
  if (filenames_.size()) {
    ifs_ = std::ifstream(filenames_[0], std::ios::binary);
  }
  return AS_OK;
}

ANLStatus ReadTelemetry::mod_analyze() {
  if (fileIndex_ >= static_cast<int>(filenames_.size())) {
    return AS_QUIT;
  }

  std::vector<uint8_t> &telemetry = Telemetry()->getContentsNC()->CommandNC();

  while (true) {
    uint8_t buffer = readOneByte();
    if (done_) break;
    setValid(false);
    const int n = telemetry.size();
    if (n >= 3 && telemetry[n - 3] == 0xEB && telemetry[n - 2] == 0x90 && telemetry[n - 1] == 0x5B && buffer == 0x6A) {
      telemetry.clear();
      telemetry.push_back(0xEB);
      telemetry.push_back(0x90);
      telemetry.push_back(0x5B);
      telemetry.push_back(0x6A);
      continue;
    }
    if (n >= 3 && telemetry[n - 3] == 0xC5 && telemetry[n - 2] == 0xA4 && telemetry[n - 1] == 0xD2 && buffer == 0x79) {
      telemetry.push_back(buffer);
      setValid(true);
      break;
    }
    telemetry.push_back(buffer);
  }

  if (Chatter() >= 1) {
    std::cout << "telemetry_size: " << telemetry.size() << std::endl;
    for (int i = 0; i < static_cast<int>(telemetry.size()); i++) {
      std::cout << "telemetry[" << i << "] = " << static_cast<int>(telemetry[i]) << std::endl;
    }
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(sleepms_));

  return AS_OK;
}

ANLStatus ReadTelemetry::mod_finalize() {
  return AS_OK;
}

uint8_t ReadTelemetry::readOneByte() {
  if (ifs_.eof()) {
    fileIndex_++;
    ifs_.close();
    if (fileIndex_ >= static_cast<int>(filenames_.size())) {
      done_ = true;
      return 0;
    }
    ifs_ = std::ifstream(filenames_[fileIndex_]);
  }
  char c;
  ifs_.read(&c, 1);
  return c;
}

} // namespace gramsballoon
#endif