#include "EncodedSerialCommunication.hh"
#include <chrono>
#include <cstdint>
#include <errno.h>
#include <string>
#include <iostream>

namespace gramsballoon::pgrams {
int EncodedSerialCommunication::ReadDataUntilBreak(std::string &data, int max_length) {
  return ReadDataUntilSpecificStr(data, "\n", max_length);
}
int EncodedSerialCommunication::ReadDataUntilSpecificStr(std::string &data, const std::string &end, int max_length) {
  using std::chrono::steady_clock;
  data.clear();
  uint8_t buf;
  const auto sz_end = end.size();
  auto deadline = steady_clock::now() + Timeout();
  data.reserve(max_length);
  while (deadline > steady_clock::now() && static_cast<int>(data.size()) < max_length && data.size() < data.capacity()) {
    const int ret_to = waitForReceivable(std::chrono::duration_cast<std::chrono::microseconds>(deadline - steady_clock::now()));
    const int err_to = errno;
    if (ret_to < 0 && (err_to == EINTR || err_to == EAGAIN)) {
      continue;
    }
    else if (ret_to < 0) {
      return ret_to;
    }
    else if (ret_to == 0) {
      break;
    }

    const int ret = sread(&buf, 1);
    const int err = errno;
    if (ret < 0 && (err == EINTR || err == EAGAIN)) {
      continue;
    }
    else if (ret < 0) {
      return ret;
    }
    else if (ret == 0) {
      break;
    }

    data += static_cast<char>(buf);
    deadline = steady_clock::now() + Timeout();
    const auto sz = data.size();
    if (sz >= sz_end &&
        data.compare(sz - sz_end, sz_end, end) == 0) { break; }
  }
  return static_cast<int>(data.size());
}

int EncodedSerialCommunication::ReadExactly(std::string &data, int length) {
  return impl(
      [this](uint8_t *d, int l) {
        return ReadExactly(d, l);
      },
      data, length);
}

int EncodedSerialCommunication::Read(std::string &data, int length) {
  return impl(
      [this](uint8_t *d, int l) {
        return Read(d, l);
      },
      data, length);
}

int EncodedSerialCommunication::Write(std::string_view data) {
  return Write(reinterpret_cast<const uint8_t *>(data.data()), data.size());
}

template <typename FUNC>
int EncodedSerialCommunication::impl(FUNC func, std::string &data, int length) {
  if (length <= 0) {
    data.clear();
    return 0;
  }
  data.resize(length);
  const int ret = func(reinterpret_cast<uint8_t *>(data.data()), length);
  if (ret >= 0) {
    data.resize(ret);
  }
  else {
    data.resize(0);
  }
  return ret;
}
} // namespace gramsballoon::pgrams