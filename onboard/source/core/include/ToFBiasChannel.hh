#ifndef GB_ToFBiasChannel_hh
#define GB_ToFBiasChannel_hh 1
#include <bitset>
#include <climits>
#include <string>
namespace gramsballoon::pgrams {
template <typename T1, typename... Ts>
struct is_all_same: std::conjunction<std::is_same<T1, Ts>...> {};

/**
 * @brief Utility class for commanding ToF Bias
 * @author Shota Arai
 * @date 2026-07-06 | Shota Arai | First version
 * 
 */
template <size_t N>
class ToFBiasChannel final {
public:
  ToFBiasChannel() : bits_(0) {}
  ~ToFBiasChannel() = default;

private:
  std::bitset<N> bits_;

public:
  template <typename... ARGS>
  void setValue(ARGS... args);

  std::string serialize() const;

private:
  template <size_t I, typename HEAD, typename... Tail>
  void setValueImpl(HEAD head, Tail... tail);
};

template <size_t N>
template <typename... ARGS>
inline void ToFBiasChannel<N>::setValue(ARGS... args) {
  static_assert(std::conjunction<std::is_integral<ARGS>...>::value, "Argument must be integer.");
  static_assert(is_all_same<ARGS...>::value, "Argument type must be same");
  bits_.reset();
  setValueImpl<N>(args...);
}

template <size_t N>
template <size_t I, typename HEAD, typename... Tail>
inline void ToFBiasChannel<N>::setValueImpl(HEAD head, Tail... tail) {
  constexpr size_t BIT_OFFSET = I - (sizeof(HEAD) * CHAR_BIT); // all types are the same.
  static_assert(BIT_OFFSET < N && BIT_OFFSET >= 0);
  bits_ |= (std::bitset<N>(head) << (BIT_OFFSET));
  if constexpr (BIT_OFFSET > 0) {
    setValueImpl<BIT_OFFSET>(tail...);
  }
}

template <size_t N>
inline std::string ToFBiasChannel<N>::serialize() const {
  if (bits_.all()) {
    return "all";
  }
  else if (bits_.none()) {
    return "";
  }
  std::string ret = " ";
  for (size_t i = 0; i < N; i++) {
    if (bits_[i]) {
      ret += std::to_string(i) + " ";
    }
  }
  return ret;
}

} // namespace gramsballoon::pgrams
#endif //GB_ToFBiasChannel_hh