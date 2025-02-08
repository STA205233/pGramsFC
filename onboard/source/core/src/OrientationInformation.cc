#include "OrientationInformation.hh"
#include <iostream>
namespace gramsballoon::pgrams {
OrientationInformation::OrientationInformation() {
  const std::map<std::string, std::string> labels = {
      {"Temperature", "T"},
      {"Magnet_x", "Mx"},
      {"Magnet_y", "My"},
      {"Magnet_z", "Mz"},
      {"Accel_x", "Ax"},
      {"Accel_y", "Ay"},
      {"Accel_z", "Az"},
      {"Gyro_x", "Gx"},
      {"Gyro_y", "Gy"},
      {"Gyro_z", "Gz"},
      {"Humidity", "H"},
  };
  for (auto &&label: labels) {
    regs_[label.first] = std::regex(label.second + "_([0-9.]+)");
    values_[label.first] = 0.0;
    valids_[label.first] = false;
  }
}

bool OrientationInformation::SetString(const std::string &string) {
  bool ret = true;
  for (const auto &reg: regs_) {
    std::smatch match;
    if (std::regex_search(string, match, reg.second)) {
      if (chatter_ > 0) {
        std::cout << reg.first << ": " << match[1] << std::endl;
      }
      if (chatter_ > 1) {
        std::cout << "matches for " << reg.first << ": " << std::endl;
        for (const auto &m: match) {
          std::cout << m << " ";
        }
        std::cout << std::endl;
      }
      try {
        values_[reg.first] = std::stod(match[1]);
        valids_[reg.first] = true;
      }
      catch (const std::invalid_argument &e) {
        std::cerr << "OrientationInformation::SetString: " << e.what() << std::endl;
        std::cerr << "OrientationInformation::SetString: " << string << std::endl;
        values_[reg.first] = 0.0;
        ret = false;
        valids_[reg.first] = true;
      }
    }
    else {
      ret = false;
      valids_[reg.first] = true;
      values_[reg.first] = 0.0;
    }
  }
  return ret;
}
double OrientationInformation::GetValue(const std::string &label) const {
  try {
    return values_.at(label);
  }
  catch (const std::out_of_range &e) {
    std::cerr << "OrientationInformation::GetValue: " << e.what() << std::endl;
  }
  std::cerr << "OrientationInformation::GetValue: " << label << " is not found" << std::endl;
  return 0.0;
}
} // namespace gramsballoon::pgrams