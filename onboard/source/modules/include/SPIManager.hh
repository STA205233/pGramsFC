/**
 * SPI Manager
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 * @date 2025-05-02 | Shota Arai | Modified to use the FT232H
 */

#ifndef SPIManager_H
#define SPIManager_H 1

#include "SPIInterface.hh"
#include "SendTelemetry.hh"
#include "ftd2xx.h"
#include <anlnext/BasicModule.hh>

namespace gramsballoon {

class SendTelemetry;

namespace pgrams {
class SPIManager: public anlnext::BasicModule {
  DEFINE_ANL_MODULE(SPIManager, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  SPIManager();
  virtual ~SPIManager();

protected:
  SPIManager(const SPIManager &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  std::shared_ptr<SPIInterface> &Interface() { return (singleton_self()->interface_); }

private:
  std::shared_ptr<SPIInterface> interface_ = nullptr;
  int channel_ = 0;
  int baudrate_ = 100000;
  int spiConfigOptions_ = 0;
  std::vector<int> chipSelectArray_;
  SendTelemetry *sendTelemetry_ = nullptr;
  std::string interfaceName_ = "BayCat";
  int chatter_ = 0;
};
} /* namespace pgrams */
} /* namespace gramsballoon */

#endif /* SPIManager_H */
