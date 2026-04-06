#ifndef GB_InclinometerController_hh
#define GB_InclinometerController_hh 1
#include "SerialCommunication.hh"
namespace gramsballoon::pgrams {
/**
 * A class to control inclinometer (VS series by level developments)
 *
 * @author Shota Arai
 * @date 2026-04-05 | Shota Arai | First version
 */
class InclinometerController: public SerialCommunication {
public:
  InclinometerController();
  InclinometerController(const std::string &serial_path, speed_t baudrate);
  virtual ~InclinometerController() = default;

protected:
  void setFlags(tcflag_t &c_cflag) override;

private:
  using SerialCommunication::sread;
  using SerialCommunication::sreadSingle;
  using SerialCommunication::swrite;
  int writeRead(const uint8_t *buf, int length, uint8_t *read_buf, int read_length);
  timeval timeout_;

public:
  int getXY(int32_t &x, int32_t &y);
  int getTemp(int16_t &temp);
  void setTimeout(int timeout_s, int timeout_us) {
    timeout_.tv_sec = timeout_s;
    timeout_.tv_usec = timeout_us;
  }
};
} // namespace gramsballoon::pgrams
#endif //GB_InclinometerController_hh