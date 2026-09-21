#ifndef GB_IRIDIUM_PACKET_POOL_HH
#define GB_IRIDIUM_PACKET_POOL_HH 1
#include "BaseTelemetryDefinition.hh"
#include <memory>
#include <optional>
#include <queue>

namespace gramsballoon::pgrams {
/**
 * @brief Class for managing Iridium packets with an option to overwrite a packet with a specific code.
 * @author Shota Arai
 * @date 2025-10-16 | First implementation.
 */
class IridiumPacketPool {
public:
  IridiumPacketPool() {
    overwrittenPacket_.reset();
  }
  virtual ~IridiumPacketPool() = default;
  /**
   * @brief Push a packet to the pool
   * @param[in] packet Packet to be pushed
   * @param[in] overwrite Set true to keep only the latest packet of this kind, which replaces the one pushed before instead of being queued
   */
  void push(const std::shared_ptr<BaseTelemetryDefinition> &packet, bool overwrite = false);

  /**
   * @brief Remove the packet at the head of the pool
   */
  void pop();
  /**
   * @brief Return whether the pool has no packet
   * @return True if the pool is empty
   */
  bool empty() const {
    if (overwrittenPacket_) {
      return false;
    }
    return packetQueue_.empty();
  }
  /**
   * @brief Return the packet at the head of the pool
   * @return Packet at the head, or nullptr if the pool is empty
   */
  std::shared_ptr<BaseTelemetryDefinition> front() const {
    if (overwrittenPacket_ && overwrittenPacketIndex_ == 0) {
      return overwrittenPacket_;
    }
    if (!packetQueue_.empty()) {
      return packetQueue_.front();
    }
    return nullptr;
  }
  /**
   * @brief Return the number of the packets in the pool
   * @return Number of the packets
   */
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