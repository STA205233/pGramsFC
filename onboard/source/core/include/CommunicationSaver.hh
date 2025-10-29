#ifndef GB_CommandWriter_hh
#define GB_CommandWriter_hh 1
#include "BinaryFileManipulater.hh"
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <iomanip>
namespace gramsballoon {
class BinaryFileManipulater;
}
namespace gramsballoon::pgrams {
template <typename ContentType>
class CommunicationSaver {
public:
  CommunicationSaver() = default;
  virtual ~CommunicationSaver();
  std::unique_ptr<std::ofstream> openFile(int type, const std::string &type_str);
  void writeCommandToFile(bool failed, const ContentType &command);
  void setNumCommandPerFile(int n) { numCommandPerFile_ = n; }
  void setBinaryFilenameBase(const std::string &base) { binaryFilenameBase_ = base; }
  void setRunID(int runId) {
    std::ostringstream run_id_sout;
    run_id_sout << std::setfill('0') << std::right << std::setw(6) << runId;
    runIdStr_ = run_id_sout.str();
  }
  void setTimeStampStr(const std::string &timeStamp) { timeStampStr_ = timeStamp; }

private:
  void write(std::ofstream *file, const ContentType &command);
  std::map<int, std::tuple<int, int, std::unique_ptr<std::ofstream>>> fileMap_;
  int numCommandPerFile_ = 100;
  std::string binaryFilenameBase_ = "";
  std::string runIdStr_ = "";
  std::string timeStampStr_ = "YYYYMMDDHHMMSS";
};
template <typename ContentType>
CommunicationSaver<ContentType>::~CommunicationSaver() {
  for (auto &[type, tuple]: fileMap_) {
    if (std::get<2>(tuple)) {
      std::get<2>(tuple)->close();
    }
  }
}

template <typename ContentType>
std::unique_ptr<std::ofstream> CommunicationSaver<ContentType>::openFile(int type, const std::string &type_str) {
  std::ostringstream filename_sout;
  filename_sout << binaryFilenameBase_ << "_" << runIdStr_ << "_" << timeStampStr_ << "_" << type_str << "_" << std::setfill('0') << std::right << std::setw(6) << std::get<0>(fileMap_[type]) << ".dat";
  return std::make_unique<std::ofstream>(filename_sout.str(), std::ios::binary);
}
template <typename ContentType>
void CommunicationSaver<ContentType>::write(std::ofstream *file, const ContentType &command) {
  if (!file) {
    std::cerr << "CommunicationSaver::write error: nullptr." << std::endl;
    return;
  }
  if (!(*file)) {
    std::cerr << "CommunicationSaver::write error: file stream error." << std::endl;
    return;
  }
  file->write(reinterpret_cast<const char *>(command.data()), command.size() * sizeof(typename ContentType::value_type));
}
template <>
inline void CommunicationSaver<CommunicationFormat>::write(std::ofstream *file, const CommunicationFormat &command) {
  if (!file) {
    std::cerr << "CommunicationSaver::write error: nullptr." << std::endl;
    return;
  }
  if (!(*file)) {
    std::cerr << "CommunicationSaver::write error: file stream error." << std::endl;
    return;
  }
  const std::vector<uint8_t> &cmd = command.Command();
  file->write(reinterpret_cast<const char *>(cmd.data()), cmd.size() * sizeof(uint8_t));
}
template <typename ContentType>
void CommunicationSaver<ContentType>::writeCommandToFile(bool failed, const ContentType &command) {
  int type = 1;
  std::string type_str = "";
  if (failed) {
    type = 0;
    type_str = "failed";
  }
  else {
    type = 1;
    type_str = "normal";
  }
  if (fileMap_.find(type) == fileMap_.end()) {
    fileMap_[type] = std::make_tuple(0, 0, nullptr);
    std::get<2>(fileMap_[type]) = openFile(type, type_str);
  }
  else if (std::get<1>(fileMap_[type]) == numCommandPerFile_) {
    std::get<0>(fileMap_[type])++;
    std::get<1>(fileMap_[type]) = 0;
    std::get<2>(fileMap_[type]) = openFile(type, type_str);
  }
  if (!std::get<2>(fileMap_[type])) {
    return;
  }
  write(std::get<2>(fileMap_[type]).get(), command);
  std::get<1>(fileMap_[type])++;
}

} // namespace gramsballoon::pgrams

#endif //GB_CommandWriter_hh