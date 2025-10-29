#include "IridiumPacketPool.hh"
namespace gramsballoon::pgrams {
void IridiumPacketPool::push(const std::shared_ptr<BaseTelemetryDefinition> &packet, bool overwrite) {
  if (packet == nullptr) {
    std::cout << "IridiumPacketPool::push: packet is nullptr." << std::endl;
    return;
  }
  if (overwrite) {
    overwrittenPacket_ = packet;
    overwrittenPacketIndex_ = packetQueue_.size();
    size_ = overwrittenPacketIndex_ + 1;
    return;
  }
  packetQueue_.push(packet);
  size_++;
}
void IridiumPacketPool::pop() {
  if (overwrittenPacket_ && overwrittenPacketIndex_ == 0) {
    overwrittenPacket_.reset();
    overwrittenPacketIndex_ = 0;
    size_--;
    return;
  }
  if (!packetQueue_.empty()) {
    packetQueue_.pop();
    if (overwrittenPacketIndex_ > 0) {
      overwrittenPacketIndex_--;
    }
    size_--;
  }
}
} // namespace gramsballoon::pgrams