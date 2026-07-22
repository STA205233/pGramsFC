#include "SPIInterfaceMultiplexer.hh"
#include "VCSMapping.hh"
#include <cstdint>
namespace gramsballoon::pgrams {
std::optional<VCSMapping::pair_t> SPIInterfaceMultiplexer::getMappingChipSelect(int multiplexerChannel) const {
  return csMapping_ ? csMapping_->getChipSelect(multiplexerChannel) : std::nullopt;
}

int SPIInterfaceMultiplexer::controlGPIO(int cs, bool value) {
  const auto mapped = getMappingChipSelect(cs);
  if (!mapped.has_value()) {
    return -1;
  }
  const auto default_val = getDefaultState();
  if (!default_val.has_value()) {
    return -1;
  }
  const uint32_t written_value = value ? mapped->second : default_val.value();
  return controlGPIOBit(mapped->first, written_value);
}

int SPIInterfaceMultiplexer::controlGPIOBit(uint32_t cs, uint32_t value) {
  return baseInterface_ ? baseInterface_->controlGPIOBit(cs, value) : -1;
}

int SPIInterfaceMultiplexer::Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) {
  return executeFunction(cs, csControl, [&](int mappedCs) {
    return baseInterface_ ? baseInterface_->Write(mappedCs, writeBuffer, size, false) : -1;
  });
}

int SPIInterfaceMultiplexer::WriteThenRead(
    int cs, const uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer, unsigned int rsize, bool csControl) {
  return executeFunction(cs, csControl, [&](int mappedCs) {
    return baseInterface_ ? baseInterface_->WriteThenRead(mappedCs, writeBuffer, wsize, readBuffer, rsize, false) : -1;
  });
}

int SPIInterfaceMultiplexer::WriteAndRead(
    int cs, uint8_t *writeBuffer, unsigned int size, uint8_t *readBuffer, bool csControl) {
  return executeFunction(cs, csControl, [&](int mappedCs) {
    return baseInterface_ ? baseInterface_->WriteAndRead(mappedCs, writeBuffer, size, readBuffer, false) : -1;
  });
}
int SPIInterfaceMultiplexer::Open(int channel) {
  if (baseInterface_) {
    const auto status = baseInterface_->Open(channel);
    if (status == 0) {
      setIsOpen(true);
    }
    return status;
  }
  return -1;
}
int SPIInterfaceMultiplexer::Close() {
  if (!IsOpen()) {
    return 0;
  }
  if (baseInterface_) {
    const auto status = baseInterface_->Close();
    if (status == 0) {
      setIsOpen(false);
    }
    return status;
  }
  return -1;
}
} // namespace gramsballoon::pgrams