#ifndef GB_HKDataSaver_h
#define GB_HKDataSaver_h 1
#include <cstdint>
#include <fstream>
namespace gramsballoon::pgrams {
template <typename DataType = uint8_t>
class HKDataSaver {
public:
  HKDataSaver(uint32_t numDataPerFile = 100000, const std::string &filenameBase = "hk_data", uint32_t Ndata = 1) : file_(), dataCount_(0), fileIndex_(0), filenameBase_(filenameBase), numDataPerFile_(numDataPerFile), NData_(Ndata) {};
  virtual ~HKDataSaver() = default;

private:
  std::ofstream file_;
  uint32_t NData_ = 1;
  uint32_t dataCount_ = 0;
  uint32_t fileIndex_ = 0;
  std::string filenameBase_ = "hk_data";
  uint32_t numDataPerFile_ = 100000;

public:
  void setFilenameBase(const std::string &base) { filenameBase_ = base; }
  void saveData(const DataType (&data));
  void openFileForWrite();
  void openFileForRead(const std::string &filename) {
    if (file_.is_open()) {
      file_.close();
    }
    file_.open(filename, std::ios::binary);
    if (!file_) {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return;
    }
    file_.read(reinterpret_cast<char *>(&NData), sizeof(uint32_t));
  }
  void closeFile() {
    if (file_.is_open()) {
      file_.close();
    }
  }
};

void HKDataSaver::saveData() {
  if (!file_.is_open() || dataCount_ >= numDataPerFile_) {
    openFileForWrite();
  }
  time_t current_time = time(nullptr);
  file_.write(reinterpret_cast<const uint8_t *>(&current_time), sizeof(current_time));
  file_.write(reinterpret_cast<const uint8_t *>(data), sizeof(DataType) * NData);
  dataCount_++;
}

void HKDataSaver::openFileForWrite() {
  if (file_.is_open()) {
    file_.close();
  }
  std::ostringstream filename_sout;
  filename_sout << filenameBase_ << "_" << std::setfill('0') << std::right << std::setw(6) << fileIndex_ << ".dat";
  file_.open(filename_sout.str(), std::ios::binary);
  if (!file_) {
    std::cerr << "Failed to open file: " << filename_sout.str() << std::endl;
    return;
  }
  fileIndex_++;
  dataCount_ = 0;
  file_.write(reinterpret_cast<const uint8_t *>(&NData), sizeof(uint32_t));
}
} // namespace gramsballoon::pgrams

#endif //GB_HKDataSaver_h