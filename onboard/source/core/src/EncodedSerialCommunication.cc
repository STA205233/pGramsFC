#include "EncodedSerialCommunication.hh"
namespace gramsballoon::pgrams {
int EncodedSerialCommunication::ReadData(std::string &data, int length) {
  data.clear();
  buf_.resize(length);
  const int ret = sread(buf_, length);
  if (ret < 0) {
    return ret;
  }
  data.assign(buf_.begin(), buf_.begin() + ret);
  return ret;
}
int EncodedSerialCommunication::ReadData(char *data, int length) {
  buf_.clear();
  buf_.resize(length);
  const int ret = sread(buf_, length);
  if (ret < 0) {
    return ret;
  }
  std::copy(buf_.begin(), buf_.begin() + ret, data);
  return ret;
}
int EncodedSerialCommunication::ReadDataUntilBreak(std::string &data) {
  return ReadDataUntilSpecificStr(data, "\n");
}
int EncodedSerialCommunication::ReadDataUntilSpecificStr(std::string &data, const std::string &end) {
  data.clear();
  uint8_t buf;
  int cnt = 0;
  const int sz_end = end.size();
  int cnt_end = 0;
  while (true) {
    if (cnt_end == sz_end) {
      break;
    }
    const int ret = sreadSingle(buf);
    if (ret < 0) {
      return ret;
    }
    data += static_cast<char>(buf);
    cnt++;
    if (cnt_end > 0 && static_cast<char>(buf) != static_cast<char>(end[cnt_end])) {
      cnt_end = 0;
    }
    if (static_cast<char>(buf) == static_cast<char>(end[cnt_end])) {
      cnt_end++;
    }
  }
  return cnt;
}
int EncodedSerialCommunication::WriteData(const std::string &data) {
  buf_.clear();
  buf_.assign(data.begin(), data.end());
  const int ret = swrite(buf_);
  return ret;
}
} // namespace gramsballoon::pgrams