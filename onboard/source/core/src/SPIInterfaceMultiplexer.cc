#include "SPIInterfaceMultiplexer.hh"
namespace gramsballoon::pgrams {
std::optional<uint32_t> SPIInterfaceMultiplexer::getMappingChipSelect(int multiplexerChannel) const {
  return csMapping_ ? csMapping_->getChipSelect(multiplexerChannel) : std::nullopt;
}

int SPIInterfaceMultiplexer::controlGPIO(int cs, bool value) {
  const auto mapped = getMappingChipSelect(cs);
  if (!mapped.has_value()) {
    return -1;
  }

  return controlGPIO(static_cast<int>(*mapped), value);
}

int SPIInterfaceMultiplexer::Write(int cs, const uint8_t *writeBuffer, unsigned int size, bool csControl) {
  return executeFunction(cs, csControl, [&](int mappedCs) {
    return baseInterface_ ? baseInterface_->Write(mappedCs, writeBuffer, size, false) : -1;
  });
}

int SPIInterfaceMultiplexer::WriteThenRead(
    int cs, const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize, bool csControl) {
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