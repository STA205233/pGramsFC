#ifndef GRAMSBalloon_IoContextManager_hh
#define GRAMSBalloon_IoContextManager_hh 1
#include "anlnext/BasicModule.hh"
#include "boost/asio.hpp"
#include <thread>

namespace gramsballoon::pgrams {
class IoContextManager: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(IoContextManager, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  IoContextManager() = default;
  virtual ~IoContextManager() = default;

protected:
  IoContextManager(const IoContextManager &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_begin_run() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_end_run() override;
  anlnext::ANLStatus mod_finalize() override;

public:
  std::shared_ptr<boost::asio::io_context> getIoContext() {
    return singleton_self()->ioContext_;
  }
  void stopIoContext() {
    if (singleton_self()->ioContext_) {
      singleton_self()->ioContext_->stop();
    }
  }

private:
  std::shared_ptr<boost::asio::io_context> ioContext_;
  std::shared_ptr<std::thread> ioThread_;
  std::shared_ptr<std::atomic<bool>> isRunning_ = nullptr;
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_IoContextManager_hh