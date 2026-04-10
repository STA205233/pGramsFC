#ifndef GB_BayCatAPICaller_hh
#define GB_BayCatAPICaller_hh 1

#include <atomic>
#define linux 1
#include "stdbool.h" // This is needed for including VL_OSALib.h
#include "VL_OSALib.h"
namespace gramsballoon::pgrams {

/**
 * @brief A class to manage the versalogic API. It ensures that the API is initialized once.
 * @author Shota Arai
 * @date 2026-04-10 | Shota Arai | Created
 */
class BayCatAPICaller {
public:
  BayCatAPICaller();
  virtual ~BayCatAPICaller();
  BayCatAPICaller(const BayCatAPICaller &);
  int apiStatus() const { return apiStatus_.load(); }

protected:
  int startAPI();
  int stopAPI();

private:
  inline static std::atomic_int counter_{0};
  inline static std::atomic_int apiStatus_{0}; // 0: not initialized, 1: initialized, <0: failed to initialize
};
} // namespace gramsballoon::pgrams
#endif //GB_BayCatAPICaller_hh