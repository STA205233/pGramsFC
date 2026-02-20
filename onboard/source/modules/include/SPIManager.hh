
#ifndef SPIManager_H
#define SPIManager_H 1

#include "SPIInterfaceMultiplexer.hh"
#include "SendTelemetry.hh"
#include <anlnext/BasicModule.hh>

namespace gramsballoon::pgrams {

class SendTelemetry;
class SPIInterface;

/**
 * SPI Manager
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 * @date 2025-05-02 | Shota Arai | Modified to use the FT232H
 * @date 2026-02-20 | Shota Arai | Refactor to use SPIInterfaceMultiplexer and support multiple SPI control types (v2.0)
 */
class SPIManager: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(SPIManager, 2.0);
  ENABLE_PARALLEL_RUN();

public:
  SPIManager();
  virtual ~SPIManager();

protected:
  SPIManager(const SPIManager &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_pre_initialize() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  SPIInterface *Interface() { return (singleton_self()->interface_).get(); }

private:
  std::shared_ptr<SPIInterface> interface_ = nullptr;
  std::string spiControlType_ = "baycat"; // "baycat" or "ft232h"
  int channel_ = 0;
  int baudrate_ = 100000;
  int spiConfigOptions_ = 0;
  std::vector<int> chipSelectArray_;
  SendTelemetry *sendTelemetry_ = nullptr;
  int chatter_ = 0;
};
} // namespace gramsballoon::pgrams

#endif /* SPIManager_H */
