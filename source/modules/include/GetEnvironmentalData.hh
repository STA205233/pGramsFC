/**
 * Get environmental data with BME680 sensor.
 *
 * @author Tsubasa Tamba, Shota Arai
 * @date 2023-03-01
 */


#ifndef GetEnvironmentalData_H
#define GetEnvironmentalData_H 1

#include <anlnext/BasicModule.hh>
#include "SPIInterface.hh"
#include "BME680IO.hh"
#include "SPIManager.hh"
#include <chrono>

namespace gramsballoon {

class GetEnvironmentalData : public anlnext::BasicModule
{
  DEFINE_ANL_MODULE(GetEnvironmentalData, 1.0);
  ENABLE_PARALLEL_RUN();

public:
  GetEnvironmentalData();
  virtual ~GetEnvironmentalData();
protected:
  GetEnvironmentalData(const GetEnvironmentalData& r) = default;

public:  
  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_pre_initialize() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;
  anlnext::ANLStatus mod_finalize() override;

  BME680IO* GetBME680IO() { return bme680io_.get(); }
  double Temperature() { return temperature_; }
  double Humidity() { return humidity_; }
  double Pressure() { return pressure_; }

private:
  int chipSelect_ = 8;
  std::string SPIManagerName_ = "SPIManager";
  SPIManager* SPIManager_ = nullptr;
  std::shared_ptr<BME680IO> bme680io_ = nullptr;
  std::shared_ptr<SPIInterface> interface_ = nullptr;
  double pressure_;
  double humidity_;
  double temperature_;
  std::chrono::system_clock::time_point lastUpdateTime_;
};

} /* namespace gramsballoon */

#endif /* GetEnvironmentalData_H */
