#include "ADC128S102IO.hh"
#include "BayCatSPIIO.hh"
#include "PDUCSMapping.hh"
#include "SPIInterfaceMultiplexer.hh"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#if defined(__linux__)
#include <malloc.h>
#endif

using namespace gramsballoon::pgrams;

namespace {
constexpr uint32_t CS_BIT = 0x1f0000;

enum class Mode {
  IDLE,
  FPGA_READ,
  FPGA_WRITE,
  GPIO,
  SPI,
  ADC,
  INVALID
};

// Allocation-free: raw syscalls and stack buffers only, so that the measurement
// itself does not move the heap.
long ResidentSetSizeKB() {
  char buf[128];
  const int fd = ::open("/proc/self/statm", O_RDONLY);
  if (fd < 0) {
    return -1;
  }
  const ssize_t n = ::read(fd, buf, sizeof(buf) - 1);
  ::close(fd);
  if (n <= 0) {
    return -1;
  }
  buf[n] = '\0';
  const char *p = std::strchr(buf, ' ');
  if (p == nullptr) {
    return -1;
  }
  return std::strtol(p, nullptr, 10) * (::sysconf(_SC_PAGESIZE) / 1024);
}

long HeapInUseBytes() {
#if defined(__linux__) && defined(__GLIBC__) && defined(__GLIBC_PREREQ)
#if __GLIBC_PREREQ(2, 33)
  return static_cast<long>(mallinfo2().uordblks);
#endif
#endif
  return -1;
}

long CountMappings() {
  const int fd = ::open("/proc/self/maps", O_RDONLY);
  if (fd < 0) {
    return -1;
  }
  char buf[8192];
  long n = 0;
  ssize_t r = 0;
  while ((r = ::read(fd, buf, sizeof(buf))) > 0) {
    for (ssize_t i = 0; i < r; ++i) {
      if (buf[i] == '\n') {
        ++n;
      }
    }
  }
  ::close(fd);
  return n;
}

long CountFds() {
  DIR *dir = ::opendir("/proc/self/fd");
  if (dir == nullptr) {
    return -1;
  }
  long n = 0;
  while (::readdir(dir) != nullptr) {
    ++n;
  }
  ::closedir(dir);
  return n - 2; // . and ..
}

Mode ParseMode(const std::string &s) {
  if (s == "idle") return Mode::IDLE;
  if (s == "fpga_read") return Mode::FPGA_READ;
  if (s == "fpga_write") return Mode::FPGA_WRITE;
  if (s == "gpio") return Mode::GPIO;
  if (s == "spi") return Mode::SPI;
  if (s == "adc") return Mode::ADC;
  return Mode::INVALID;
}

void PrintUsage(const char *argv0) {
  std::cerr << "Usage: " << argv0 << " <mode> [iterations] [report_every]" << std::endl;
  std::cerr << "  idle       : no VersaLogic API call (control)" << std::endl;
  std::cerr << "  fpga_read  : FPGAReadRegister(AUX_IN)          x1 API call per iteration" << std::endl;
  std::cerr << "  fpga_write : FPGAWriteRegister(AUX_OUT)        x1 API call per iteration" << std::endl;
  std::cerr << "  gpio       : controlGPIOBit(0x1f0000)          x5 API calls per iteration" << std::endl;
  std::cerr << "  spi        : WriteAndRead without CS control   x4 API calls per iteration" << std::endl;
  std::cerr << "  adc        : same sequence as GetPDUInfo       x28 API calls per iteration" << std::endl;
}
} // namespace

int main(int argc, char *argv[]) {
  if (argc < 2) {
    PrintUsage(argv[0]);
    return -1;
  }
  const Mode mode = ParseMode(argv[1]);
  if (mode == Mode::INVALID) {
    PrintUsage(argv[0]);
    return -1;
  }
  const std::string mode_name = argv[1];
  const long iterations = (argc > 2) ? std::stol(argv[2]) : 100000;
  const long report_every = (argc > 3) ? std::stol(argv[3]) : 10000;

  auto baycat = std::make_shared<BayCatSPIIO>();
  baycat->setConfigOptions(BayCatSPIIO::MakeOption(2, 0));
  baycat->setBaudrate(100000);
  if (baycat->Open(0, "") != 0) {
    std::cerr << "Open failed" << std::endl;
    return -1;
  }

  auto mux = std::make_unique<SPIInterfaceMultiplexer>();
  std::shared_ptr<SPIInterface> base = baycat;
  mux->setBaseInterface(base);
  mux->setMappingChipSelect(std::make_unique<PDUCSMapping>(CS_BIT));
  mux->Open(0, "");
  ADC128S102IO adc(5.0);
  adc.setSPIInterface(mux.get());

  unsigned char aux_out = 0;
  baycat->ReadFPGARegister(BayCatSPIIO::AUX_OUT, &aux_out);
  uint8_t wbuf[2] = {0, 0};
  uint8_t rbuf[2] = {0, 0};

  // Warm up the allocator so that start-up allocations are not counted as growth.
  for (int i = 0; i < 100; ++i) {
    ResidentSetSizeKB();
  }
  const long rss_start = ResidentSetSizeKB();
  const long heap_start = HeapInUseBytes();
  const long maps_start = CountMappings();
  const long fds_start = CountFds();
  std::cout << "mode=" << mode_name << " iterations=" << iterations
            << " rss=" << rss_start << " kB"
            << " heap=" << heap_start << " B"
            << " maps=" << maps_start
            << " fds=" << fds_start << std::endl;
  const auto time_start = std::chrono::steady_clock::now();

  for (long i = 0; i < iterations; ++i) {
    switch (mode) {
    case Mode::IDLE:
      break;
    case Mode::FPGA_READ: {
      unsigned char value = 0;
      baycat->ReadFPGARegister(BayCatSPIIO::AUX_IN, &value);
      break;
    }
    case Mode::FPGA_WRITE:
      baycat->WriteFPGARegister(BayCatSPIIO::AUX_OUT, aux_out);
      break;
    case Mode::GPIO:
      baycat->controlGPIOBit(CS_BIT, 0);
      break;
    case Mode::SPI:
      baycat->WriteAndRead(-1, wbuf, 2, rbuf, false);
      break;
    case Mode::ADC:
      adc.setCS(static_cast<int>(i % 10));
      adc.getCurrentVoltageADC(static_cast<int>(i % 8));
      adc.resetError();
      break;
    default:
      break;
    }

    if ((i + 1) % report_every == 0) {
      const long rss = ResidentSetSizeKB();
      const long heap = HeapInUseBytes();
      const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - time_start).count();
      std::cout << "iter=" << (i + 1)
                << " rss=" << rss << " kB (" << (rss - rss_start) << ")"
                << " heap=" << heap << " B (" << (heap - heap_start) << ")"
                << " rate=" << static_cast<long>((i + 1) / elapsed) << " /s"
                << std::endl;
    }
  }

  const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - time_start).count();
  const long rss_end = ResidentSetSizeKB();
  const long heap_end = HeapInUseBytes();
  std::cout << "mode=" << mode_name
            << " rss_growth=" << (rss_end - rss_start) << " kB"
            << " heap_growth=" << (heap_end - heap_start) << " B"
            << " maps_growth=" << (CountMappings() - maps_start)
            << " fds_growth=" << (CountFds() - fds_start)
            << " heap_bytes_per_iteration=" << static_cast<double>(heap_end - heap_start) / iterations
            << " elapsed=" << elapsed << " s" << std::endl;

  mux->Close();
  return 0;
}
