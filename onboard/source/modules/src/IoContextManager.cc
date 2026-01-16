#include "IoContextManager.hh"
using namespace anlnext;
namespace gramsballoon::pgrams {

ANLStatus IoContextManager::mod_define() {
  return AS_OK;
}
ANLStatus IoContextManager::mod_initialize() {
  isRunning_ = std::make_shared<std::atomic<bool>>(false);
  ioContext_ = std::make_shared<boost::asio::io_context>();
  return AS_OK;
}
ANLStatus IoContextManager::mod_begin_run() {
  if (ioContext_) {
    ioThread_ = std::make_shared<std::thread>([this]() {
      ioContext_->run();
    });
    isRunning_->store(true, std::memory_order_release);
  }
  return AS_OK;
}
ANLStatus IoContextManager::mod_analyze() {
  return AS_OK;
}
ANLStatus IoContextManager::mod_end_run() {
  if (ioContext_) {
    ioContext_->stop();
  }
  if (ioThread_ && ioThread_->joinable()) {
    ioThread_->join();
  }
  return AS_OK;
}
ANLStatus IoContextManager::mod_finalize() {

  return AS_OK;
}
} // namespace gramsballoon::pgrams