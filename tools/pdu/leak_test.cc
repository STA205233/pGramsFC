#include "ADC128S102IO.hh"
#include "BayCatSPIIO.hh"
#include "PDUCSMapping.hh"
#include "SPIInterfaceMultiplexer.hh"
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

using namespace gramsballoon::pgrams;

namespace {
constexpr uint32_t CS_BIT = 0x1f0000;

long ResidentSetSizeKB() {
  std::ifstream ifs("/proc/self/statm");
  if (!ifs) {
    return -1;
  }
  long total = 0;
  long resident = 0;
  ifs >> total >> resident;
  return resident * (sysconf(_SC_PAGESIZE) / 1024);
}

long CountLines(const std::string &path) {
  std::ifstream ifs(path);
  if (!ifs) {
    return -1;
  }
  long n = 0;
  std::string line;
  while (std::getline(ifs, line)) {
    ++n;
  }
  return n;
}

long CountFds() {
  std::error_code ec;
  long n = 0;
  const std::filesystem::directory_iterator end;
  for (auto it = std::filesystem::directory_iterator("/proc/self/fd", ec); !ec && it != end; it.increment(ec)) {
    ++n;
  }
  return ec ? -1 : n;
}

void PrintUsage(const char *argv0) {
  std::cerr << "Usage: " << argv0 << " <mode> [iterations] [report_every]" << std::endl;
  std::cerr << "  idle       : no VersaLogic API call (control)" << std::endl;
  std::cerr << "  fpga_read  : FPGAReadRegister(AUX_IN)              x1 per iteration" << std::endl;
  std::cerr << "  fpga_write : FPGAWriteRegister(AUX_OUT)            x1 per iteration" << std::endl;
  std::cerr << "  gpio       : controlGPIOBit(0x1f0000)              x5 register calls per iteration" << std::endl;
  std::cerr << "  spi        : WriteAndRead without CS control       x4 SPI frame calls per iteration" << std::endl;
  std::cerr << "  adc        : same sequence as GetPDUInfo           x1 channel read per iteration" << std::endl;
}
} // namespace

int main(int argc, char *argv[]) {
  if (argc < 2) {
    PrintUsage(argv[0]);
    return -1;
  }
  const std::string mode = argv[1];
  const long iterations = (argc > 2) ? std::stol(argv[2]) : 100000;
  const long report_every = (argc > 3) ? std::stol(argv[3]) : 5000;

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

  const long rss_start = ResidentSetSizeKB();
  const auto time_start = std::chrono::steady_clock::now();
  std::cout << "mode=" << mode << " iterations=" << iterations
            << " rss_start=" << rss_start << " kB"
            << " maps=" << CountLines("/proc/self/maps")
            << " fds=" << CountFds() << std::endl;

  for (long i = 0; i < iterations; ++i) {
    if (mode == "idle") {
      ;
    }
    else if (mode == "fpga_read") {
      unsigned char value = 0;
      baycat->ReadFPGARegister(BayCatSPIIO::AUX_IN, &value);
    }
    else if (mode == "fpga_write") {
      baycat->WriteFPGARegister(BayCatSPIIO::AUX_OUT, aux_out);
    }
    else if (mode == "gpio") {
      baycat->controlGPIOBit(CS_BIT, 0);
    }
    else if (mode == "spi") {
      baycat->WriteAndRead(-1, wbuf, 2, rbuf, false);
    }
    else if (mode == "adc") {
      adc.setCS(static_cast<int>(i % 10));
      adc.getCurrentVoltageADC(static_cast<int>(i % 8));
      adc.resetError();
    }
    else {
      PrintUsage(argv[0]);
      return -1;
    }

    if ((i + 1) % report_every == 0) {
      const long rss = ResidentSetSizeKB();
      const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - time_start).count();
      std::cout << "iter=" << (i + 1)
                << " rss=" << rss << " kB"
                << " delta=" << (rss - rss_start) << " kB"
                << " maps=" << CountLines("/proc/self/maps")
                << " fds=" << CountFds()
                << " rate=" << static_cast<long>((i + 1) / elapsed) << " /s"
                << std::endl;
    }
  }

  const long rss_end = ResidentSetSizeKB();
  const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - time_start).count();
  std::cout << "mode=" << mode
            << " rss_growth=" << (rss_end - rss_start) << " kB"
            << " bytes_per_iteration=" << (1024.0 * (rss_end - rss_start)) / iterations
            << " elapsed=" << elapsed << " s" << std::endl;

  mux->Close();
  return 0;
}
