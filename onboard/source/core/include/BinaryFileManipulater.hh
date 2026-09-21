#ifndef BinaryFileManipulater_H
#define BinaryFileManipulater_H 1

#include <iostream>
#include <vector>
#include <string>
#include <fstream>


namespace gramsballoon {
/**
 * @brief Write the contents of a vector to a binary file in the big endian order
 * @param[in] filename Name of the output file
 * @param[in] append Set true to append to the file, false to overwrite it
 * @param[in] vec Vector to be written
 */
template <typename T> void writeVectorToBinaryFile(const std::string& filename, bool append, const std::vector<T>& vec)
{
  std::ofstream ofs;
  if (append) {
    ofs = std::ofstream(filename, std::ios::app|std::ios::binary);
  }
  else {
    ofs = std::ofstream(filename, std::ios::out|std::ios::binary);
  }
  if (!ofs) {
    std::cerr << "File open error." << std::endl;
    return;
  }
  const int n = vec.size();
  const int byte = sizeof(T);
  for (int i=0; i<n; i++) {
    for (int j=0; j<byte; j++) {
      const int shift = 8*(byte-1-j);
      const char c = static_cast<char>((vec[i]>>shift)&0xff);
      ofs.write(&c, 1);
    }
  }
  ofs.flush();
  ofs.close();
}

} /* namespace gramsballoon */

#endif /* BinaryFileManipulater_H */
