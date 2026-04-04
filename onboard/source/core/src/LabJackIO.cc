#include "LabJackIO.hh"
namespace gramsballoon::pgrams {
using namespace LabJackAddresses;
int LabJackIO::open() {
  int handle;
  LJM_ERROR_RETURN err = LJM_Open(LJM_dtANY, LJM_ctANY, "ANY", &handle);
  if (err != LJME_NOERROR) {
    return err;
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
