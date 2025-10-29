/**
 * Utility classes for terminal coloring
 *
 * @author Shota Arai
 * @date 2025-03-24 | first draft
 *
 */

#ifndef GRAMSBalloon_TermianlColoring_hh
#define GRAMSBalloon_TermianlColoring_hh 1
#include <iomanip>
#include <iostream>
namespace gramsballoon::pgrams::termutil {
inline std::ostream &red(std::ostream &os) {
  os << "\033[31m";
  return os;
}
inline std::ostream &green(std::ostream &os) {
  os << "\033[32m";
  return os;
}
inline std::ostream &yellow(std::ostream &os) {
  os << "\033[33m";
  return os;
}
inline std::ostream &blue(std::ostream &os) {
  os << "\033[34m";
  return os;
}
inline std::ostream &magenta(std::ostream &os) {
  os << "\033[35m";
  return os;
}
inline std::ostream &cyan(std::ostream &os) {
  os << "\033[36m";
  return os;
}
inline std::ostream &white(std::ostream &os) {
  os << "\033[37m";
  return os;
}
inline std::ostream &black(std::ostream &os) {
  os << "\033[30m";
  return os;
}
inline std::ostream &bBlack(std::ostream &os) {
  os << "\033[40m";
  return os;
}
inline std::ostream &bRed(std::ostream &os) {
  os << "\033[41m";
  return os;
}
inline std::ostream &bGreen(std::ostream &os) {
  os << "\033[42m";
  return os;
}
inline std::ostream &bYellow(std::ostream &os) {
  os << "\033[43m";
  return os;
}
inline std::ostream &bBlue(std::ostream &os) {
  os << "\033[44m";
  return os;
}
inline std::ostream &bMagenta(std::ostream &os) {
  os << "\033[45m";
  return os;
}
inline std::ostream &bCyan(std::ostream &os) {
  os << "\033[46m";
  return os;
}
inline std::ostream &bWhite(std::ostream &os) {
  os << "\033[47m";
  return os;
}
inline std::ostream &bold(std::ostream &os) {
  os << "\033[1m";
  return os;
}
inline std::ostream &blurred(std::ostream &os) {
  os << "\033[2m";
  return os;
}
inline std::ostream &italic(std::ostream &os) {
  os << "\033[3m";
  return os;
}
inline std::ostream &underline(std::ostream &os) {
  os << "\033[4m";
  return os;
}
inline std::ostream &blink(std::ostream &os) {
  os << "\033[5m";
  return os;
}
inline std::ostream &blinkFast(std::ostream &os) {
  os << "\033[6m";
  return os;
}
inline std::ostream &reverse(std::ostream &os) {
  os << "\033[7m";
  return os;
}
inline std::ostream &hide(std::ostream &os) {
  os << "\033[8m";
  return os;
}
inline std::ostream &reset(std::ostream &os) {
  os << "\033[0m";
  return os;
}
} // namespace gramsballoon::pgrams::termutil
#endif // GRAMSBalloon_TermianlColoring_hh