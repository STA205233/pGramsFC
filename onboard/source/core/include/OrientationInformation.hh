#ifndef GRAMSBalloon_OrientationInformation_HH
#define GRAMSBalloon_OrientationInformation_HH 1
#include <map>
#include <regex>
namespace gramsballoon::pgrams {
class OrientationInformation {
public:
  OrientationInformation();
  virtual ~OrientationInformation() = default;
  OrientationInformation(const OrientationInformation &r) = default;

  bool SetString(const std::string &string);
  void SetChatter(int chatter) { chatter_ = chatter; }
  void ResetValues() {
    for (auto &value: values_) {
      value.second = 0.0;
    }
    for (auto &valid: valids_) {
      valid.second = false;
    }
  }
  double GetValue(const std::string &label) const;
  double GetTemperature() const { return GetValue("Temperature"); }
  double GetMagnetX() const { return GetValue("Magnet_x"); }
  double GetMagnetY() const { return GetValue("Magnet_y"); }
  double GetMagnetZ() const { return GetValue("Magnet_z"); }
  double GetAccelX() const { return GetValue("Accel_x"); }
  double GetAccelY() const { return GetValue("Accel_y"); }
  double GetAccelZ() const { return GetValue("Accel_z"); }
  double GetGyroX() const { return GetValue("Gyro_x"); }
  double GetGyroY() const { return GetValue("Gyro_y"); }
  double GetGyroZ() const { return GetValue("Gyro_z"); }
  double GetHumidity() const { return GetValue("Humidity"); }

private:
  std::map<std::string, std::regex> regs_;
  std::map<std::string, double> values_;
  std::map<std::string, bool> valids_;
  int chatter_ = 0;
};
} // namespace gramsballoon::pgrams
#endif //GRAMSBalloon_OrientationInformation_HH