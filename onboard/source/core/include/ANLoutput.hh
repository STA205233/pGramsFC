#ifndef GRAMSBalloon_ANLoutput_hh
#define GRAMSBalloon_ANLoutput_hh 1
#include "anlnext/BasicModule.hh"
#include <iostream>
#define MOD_OUTPUT
namespace gramsballoon::pgrams::anloutput {
inline std::ostream &anlout(const anlnext::BasicModule *mod, const std::string &func) {
#ifdef MOD_OUTPUT
  if (mod)
    std::cout << mod->get_module_id() << "::" << func << ": ";
#endif
  return std::cout;
}
} // namespace gramsballoon::pgrams::anloutput
#endif //GRAMSBalloon_ANLoutput_hh