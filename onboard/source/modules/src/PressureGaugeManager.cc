#include "PressureGaugeManager.hh"
namespace gramsballoon::pgrams {
int PressureGaugeManager::SendComAndGetData(std::string_view command, std::string &data) {
  std::shared_ptr<EncodedSerialCommunication> esc = GetEncodedSerialCommunication();
  esc->Write(command);
  const int ret = esc->ReadDataUntilSpecificStr(data, "FF");
  std::cout << "data: " << data << std::endl;
  return ret;
}
} // namespace gramsballoon::pgrams