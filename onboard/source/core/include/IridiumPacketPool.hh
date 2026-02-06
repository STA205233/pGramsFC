#ifndef GB_IRIDIUM_PACKET_POOL_HH
#define GB_IRIDIUM_PACKET_POOL_HH 1
#include "BaseTelemetryDefinition.hh"
#include <memory>
#include <optional>
#include <queue>

namespace gramsballoon::pgrams {
/**
 * Class for managing Iridium packets with an option to overwrite a packet with a specific code.
 * @author Shota Arai
 * @date 2025-10-16 | First implementation.
 */
class IridiumPacketPool {
public:
  IridiumPacketPool() {
    overwrittenPacket_.reset();
  }
  virtual ~IridiumPacketPool() = default;
  void push(const std::shared_ptr<BaseTelemetryDefinition> &packet, bool overwrite = false);

  void pop();
  bool empty() const {
    if (overwrittenPacket_) {
      return false;
    }
    return packetQueue_.empty();
  }
  std::shared_ptr<BaseTelemetryDefinition> front() const {
    if (overwrittenPacket_ && overwrittenPacketIndex_ == 0) {
      return overwrittenPacket_;
    }
    if (!packetQueue_.empty()) {
      return packetQueue_.front();
    }
    return nullptr;
  }
  size_t size() const {
    return size_;
  }

private:
  size_t size_ = 0;
  std::queue<std::shared_ptr<BaseTelemetryDefinition>> packetQueue_;
  std::shared_ptr<BaseTelemetryDefinition> overwrittenPacket_;
  size_t overwrittenPacketIndex_ = 0;
};
} // namespace gramsballoon::pgrams
#endif //GB_IRIDIUM_PACKET_POOL_HH