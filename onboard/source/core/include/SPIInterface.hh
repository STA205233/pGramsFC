#ifndef SPIInterface_H
#define SPIInterface_H 1

#include "VCSMapping.hh"
#include <cstdint>
#include <vector>
namespace gramsballoon::pgrams {
/**
 * @brief A class of SPI Interface
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 * @date 2025-05-02 | Shota Arai | Modified to use the FT232H, major refactoring
 * @date 2026-08-11 | Shota Arai | Modified slightly for implementation of the MCP2210
 */
class SPIInterface {
public:
  SPIInterface() = default;
  virtual ~SPIInterface() = default;

protected:
  SPIInterface(const SPIInterface &) = delete;
  void setIsOpen(bool isOpen) { isOpen_ = isOpen; }

public:
  unsigned int Baudrate() const { return baudrate_; }
  virtual int updateSetting() { return -1; }
  virtual void setBaudrate(unsigned int baudrate) {
    baudrate_ = baudrate;
  }
  bool IsOpen() const { return isOpen_; }
  virtual void setConfigOptions(unsigned int configOptions) { configOptions_ = configOptions; }
  unsigned int ConfigOptions() const { return configOptions_; }
  virtual int Open(int, const char * = "") { return -1; }
  virtual int Close() { return -1; }
  virtual int WriteThenRead(int, const uint8_t *, unsigned int, uint8_t *, unsigned int, bool = true) { return -1; }
  virtual int WriteAndRead(int, uint8_t *, unsigned int, uint8_t *, bool = true) { return -1; }
  virtual int Write(int, const uint8_t *, unsigned int, bool = true) { return -1; }
  virtual int controlGPIO(int, bool) { return -1; }
  /**
   *  @brief Control GPIO specified by bit expression
   *  @param csBit Set high to be controlled
   *  @param state Specify high / low
   */
  virtual int controlGPIOBit(uint32_t, uint32_t) { return -1; }
  virtual int MaximumCh() const { return 0; }

  // Convenience functions that use the cs_ member variable
  int WriteThenRead(const uint8_t *writeBuffer, int wsize, uint8_t *readBuffer, int rsize) {
    return WriteThenRead(cs_, writeBuffer, wsize, readBuffer, rsize);
  }
  int WriteAndRead(uint8_t *writeBuffer, unsigned int wsize, uint8_t *readBuffer) {
    return WriteAndRead(cs_, writeBuffer, wsize, readBuffer);
  }
  int Write(const uint8_t *writeBuffer, unsigned int size) {
    return Write(cs_, writeBuffer, size);
  }
  int getCs() const { return cs_; }
  void setCs(int cs) {
    cs_ = cs;
    channels_ = {{static_cast<VCSMapping::cs_t>(cs_), static_cast<VCSMapping::cs_t>(cs_)}};
  }

  virtual const std::vector<VCSMapping::pair_t> &Channels() const { return channels_; }

private:
  int cs_ = -1;
  unsigned int baudrate_ = 1000000;
  unsigned int configOptions_ = 0;
  bool isOpen_ = false;
  std::vector<VCSMapping::pair_t> channels_{{static_cast<VCSMapping::cs_t>(cs_), static_cast<VCSMapping::cs_t>(cs_)}};
};

} // namespace gramsballoon::pgrams

#endif /* SPIInterface_H */
