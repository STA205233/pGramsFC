#include "LabJackIO.hh"
#include <iostream>
namespace gramsballoon::pgrams {
using namespace LabJackAddresses;
LabJackIO::LabJackIO() {
  const LJM_ERROR_RETURN ret = LJM_CloseAll();
  if (ret != 0) {
    char err_str[LJM_STRING_MAX_SIZE];
    LJM_ErrorToString(ret, err_str);
    std::cerr << "Failed to close LabJack device: " << err_str << std::endl;
  }
  else {
    std::cout << "CloseAll of Labjack successful" << std::endl;
  }
}

int LabJackIO::open() {
  int handle;
  constexpr int num_trial = 10;
  for (int i = 0; i < num_trial; ++i) {
    LJM_ERROR_RETURN err = LJM_Open(LJM_dtANY, LJM_ctANY, "ANY", &handle);
    if (err == LJME_NOERROR) {
      break;
    }
    else {
      char err_str[LJM_STRING_MAX_SIZE];
      LJM_ErrorToString(err, err_str);
      std::cerr << "Failed to open LabJack device: " << err_str << std::endl;
    }
  }
  handle_ = handle;
  return 0;
}
int LabJackIO::close() {
  if (handle_ < 0) {
    return 0; // Not open, so nothing to do
  }
  LJM_ERROR_RETURN err = LJM_Close(handle_);
  if (err != LJME_NOERROR) {
    return err;
  }
  handle_ = -1;
  return 0;
}

} // namespace gramsballoon::pgrams
