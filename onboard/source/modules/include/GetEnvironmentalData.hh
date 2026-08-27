
#ifndef GetEnvironmentalData_H
#define GetEnvironmentalData_H 1

#include "SendTelemetry.hh"
#include "VHousekeepingModule.hh"
#include <anlnext/BasicModule.hh>
#include <memory>
#include <string>

namespace gramsballoon::pgrams {

class I2CManager;
class I2CInterface;
class BME680IO;
class SendTelemetry;

/**
 * @brief Get environmental data with BME680 sensor.
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 * @date 2026-04-12 | Shota Arai | Modified for pGRAMS
 * @date 2026-08-28 | Shota Arai | Use of VHousekeepingModule
 */
class GetEnvironmentalData: public VHousekeepingModule {
  DEFINE_ANL_MODULE(GetEnvironmentalData, 2.1);
  ENABLE_PARALLEL_RUN();

public:
  GetEnvironmentalData();
  virtual ~GetEnvironmentalData();

protected:
  GetEnvironmentalData(const GetEnvironmentalData &r) = default;

public:
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_pre_initialize() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  void setDataAquisitionError();

  double Temperature() { return singleton_self()->temperature_; }
  double Humidity() { return singleton_self()->humidity_; }
  double Pressure() { return singleton_self()->pressure_; }

private:
  std::string I2CManagerName_ = "I2CManager";
  I2CManager *I2CManager_ = nullptr;
  SendTelemetry *sendTelemetry_ = nullptr;
  std::shared_ptr<BME680IO> bme680io_ = nullptr;
  std::shared_ptr<I2CInterface> interface_ = nullptr;
  double pressure_;
  double humidity_;
  double temperature_;
  int chatter_ = 0;
};

} // namespace gramsballoon::pgrams

#endif /* GetEnvironmentalData_H */
