#ifdef USE_SYSTEM_MODULES
#include "GetComputerStatus.hh"
#include <sys/vfs.h>
#include <fstream>

using namespace anlnext;

namespace gramsballoon::pgrams {

GetComputerStatus::GetComputerStatus()
{
  tempFile_ = "/sys/class/thermal/thermal_zone0/temp";
  memFile_ = "/proc/meminfo";
  path_ = "/";
}

GetComputerStatus::~GetComputerStatus() = default;

ANLStatus GetComputerStatus::mod_define()
{
  define_parameter("temperature_filename", &mod_class::tempFile_);
  define_parameter("memory_filename", &mod_class::memFile_);
  define_parameter("path", &mod_class::path_);
  define_parameter("chatter", &mod_class::chatter_);

  return AS_OK;
}

ANLStatus GetComputerStatus::mod_initialize()
{
  const std::string send_telem_md = "SendTelemetry";
  if (exist_module(send_telem_md)) {
    get_module_NC(send_telem_md, &sendTelemetry_);
  }

  return AS_OK;
}

ANLStatus GetComputerStatus::mod_analyze()
{
  std::ifstream ifstemp(tempFile_);
  ifstemp >> CPUTemperatureADC_;
  CPUTemperature_ = CPUTemperatureADC_ / 1000.0;
  ifstemp.close();

  const int status = getCapacity();
  if (status != 0) {
    std::cerr << "Error in GetComputerStatus::mod_analyze()" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::GET_CAPACITY_ERROR);
    }
  }
  const int status2 = getRAMUsage();
  if (status2 != 0) {
    std::cerr << "Error in GetComputerStatus::mod_analyze()" << std::endl;
    if (sendTelemetry_) {
      sendTelemetry_->getErrorManager()->setError(ErrorType::GET_MEM_ERROR);
    }
  }
  if (chatter_>=1) {
    constexpr uint64_t one = 1;
    std::cout << "CPU Temperature (degreeC): " << CPUTemperature_ << std::endl;
    std::cout << "Free size (MB): " << capacityFree_ / (one<<20) << std::endl;
    std::cout << "All size (MB): " << capacityAll_ / (one<<20) << std::endl;
    std::cout << "RAM size (MB): " << ramTotal_ / (one<<10) << std::endl;
    std::cout << "RAM available (MB): " << ramAvail_ / (one<<10) << std::endl;
    std::cout << "RAM usage (MB): " << ramUsage_ / (one<<10) << std::endl;
  }

  return AS_OK;
}


ANLStatus GetComputerStatus::mod_finalize()
{
  return AS_OK;
}

int GetComputerStatus::getCapacity()
{
  struct statfs64 capacity;
  int rslt = statfs64(path_.c_str(), &capacity);
  if (rslt < 0) {
    std::cerr << "failed in get_freesize(). Code: " << rslt << std::endl;
    return -1;
  }
  capacityFree_ = capacity.f_bavail * capacity.f_bsize;
  capacityAll_ = capacity.f_blocks * capacity.f_bsize;
  return 0;
}

int GetComputerStatus::getRAMUsage(){
  std::ifstream ifmem(memFile_);
  if (!ifmem) {
    std::cerr << "Failed to open memory file: " << memFile_ << std::endl;
    return -1;
  }
  std::string memstr, key, unit;
  uint8_t mem_set = 0;
  while (true) {
    if (mem_set == 3) {
      ramUsage_ = ramTotal_ - ramAvail_;
      return 0;
    }
    ifmem >> key >> memstr >> unit;
    if (ifmem.eof()) break;
    if (key.find("MemTotal:") != std::string::npos) {
      try {
        ramTotal_ = std::stoll(memstr);
        mem_set |= 2U; // MemTotal: bit 1
      }
      catch (...) {
        std::cerr << "failed parse MemTotal value" << std::endl;
        return -1;
      }
    }
    else if (key.find("MemAvailable:") != std::string::npos) {
      try {
        ramAvail_ = std::stoll(memstr);
        mem_set |= 1U; // MemAvailable: bit 0
        continue;
      }
      catch (...) {
        std::cerr << "failed parse MemAvailable value" << std::endl;
        return -1;
      }
    }
  }
  return -1;
}
} /* namespace gramsballoon */
#endif