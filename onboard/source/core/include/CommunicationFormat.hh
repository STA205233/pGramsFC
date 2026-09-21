#ifndef GB_CommunicationFormat_H
#define GB_CommunicationFormat_H 1

#include "CRC16.hh"
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/time.h>
#include <vector>

namespace gramsballoon::pgrams {

/**
  * @brief A class to define the command format for the pGRAMS system.
  * @author Tsubasa Tamba, Shota Arai
  * @date 2023-03-25
  * @date 2025-03-11 | Modified to fit the pGRAMS command format.
  * @date 2025-04-01 | Name changed from CommandDefinition to CommunicationFormat
  * @ingroup daqcom_core telecom_core
  */
class CommunicationFormat {
public:
  CommunicationFormat();

  /**
   * @brief Set data from vector<uint8_t> with validation
   * @param[in] v Data to be set
   * @return True if the data passes the validation
   */
  bool setData(const std::vector<uint8_t> &v);
  /**
   * @brief Set data from string with validation
   * @param[in] s Data to be set
   * @return True if the data passes the validation
   */
  bool setData(const std::string &s);

  /**
   * @brief Check whether the data has the correct start code and stop code
   * @param[in] v Data to be checked
   * @return True if both the start code and the stop code are correct
   */
  template <typename T>
  bool checkHeaderFooter(const T &v);
  /**
   * @brief Check whether the data has the expected length and the correct CRC16
   * @param[in] v Data to be checked
   * @param[in] argc Number of the arguments expected in the data
   * @return True if the data is valid
   */
  template <typename T>
  bool validate(const T &v, uint16_t argc);

  /**
   * @brief Extract the code and the arguments from the data
   */
  void interpret();
  /**
   * @brief Write the data as binary to the given stream
   * @param[in,out] stream Output stream to be written to
   * @return The given stream
   */
  std::ostream &write(std::ostream &stream);
  /**
   * @brief Get a value of the type T placed at the given position of the data, in the big endian order
   * @param[in] index Index of the first byte of the value
   * @return Value read from the data
   */
  template <typename T>
  T getValue(int index);
  /**
   * @brief Get successive values of the type T placed at the given position of the data
   * @param[in] index Index of the first byte of the values
   * @param[in] num Number of the values to be read
   * @param[out] vec Destination of the values
   */
  template <typename T>
  void getVector(int index, int num, std::vector<T> &vec);

  /**
   * @brief Return the read-only data including the header and the footer
   * @return Data of the command
   */
  const std::vector<uint8_t> &Command() const { return command_; }
  /**
   * @brief Return the modifiable data including the header and the footer
   * @return Data of the command
   */
  std::vector<uint8_t> &CommandNC() { return command_; }
  /**
   * @brief Copy the data into a string
   * @param[out] outStr Destination of the data
   */
  void CommandStr(std::string &outStr) const { outStr.assign(command_.begin(), command_.end()); }
  /**
   * @brief Return the code of the command
   * @return Code of the command
   */
  uint16_t Code() const { return code_; }
  /**
   * @brief Return the number of the arguments
   * @return Number of the arguments
   */
  uint16_t Argc() const { return argc_; }
  /**
   * @brief Return the arguments of the command
   * @return Arguments of the command
   */
  const std::vector<uint32_t> &Arguments() const { return arguments_; }
  /**
   * @brief Build the data from the code and the arguments, adding the header, the CRC16 and the footer
   */
  void update();
  /**
   * @brief Set the code of the command
   * @param[in] code Code of the command
   */
  void setCode(uint16_t code) {
    code_ = code;
    updated_ = false;
  }
  /**
   * @brief Set the number of the arguments and resize the argument container
   * @param[in] argc Number of the arguments
   */
  void setArgc(uint16_t argc) {
    argc_ = argc;
    arguments_.resize(argc_);
    updated_ = false;
  }
  /**
   * @brief Set all the arguments of the command
   * @param[in] arguments Arguments to be set
   */
  void setArguments(const std::vector<uint32_t> &arguments) {
    setArgc(static_cast<uint16_t>(arguments.size()));
    for (uint16_t i = 0; i < argc_; ++i) {
      arguments_[i] = arguments[i];
    }
    updated_ = false;
  }
  /**
   * @brief Set one argument of the command
   * @param[in] index Index of the argument
   * @param[in] argument Value of the argument
   */
  void setArguments(uint16_t index, uint32_t argument) {
    if (index >= argc_) {
      std::cerr << "Error in CommunicationFormat: index(" << index << ") is larger than argc (" << argc_ << ")" << std::endl;
      return;
    }
    arguments_[index] = argument;
    updated_ = false;
  }
  /**
   * @brief Return one argument of the command
   * @param[in] index Index of the argument
   * @return Value of the argument, or 0 if the index is out of range
   */
  uint32_t getArguments(uint16_t index) const {
    if (index < argc_) {
      return arguments_[index];
    }
    return 0;
  }

  /**
   * @brief Set command data directly without validation (use with caution)
   * @param[in] command Data to be set
   * @note Do not use this function when you want to interpret data. Use setData() instead.
   */
  void setCommand(const std::vector<uint8_t> &command) {
    updated_ = true;
    command_ = command;
  }

  /**
   * @brief Set command data directly without validation (use with caution)
   * @param[in] command Data to be set
   * @note Do not use this function when you want to interpret data. Use setData() instead.
   */
  void setCommand(const std::string &command) {
    command_.assign(command.begin(), command.end());
    updated_ = true;
  }
  /**
   * @brief Print the command details to the given stream
   * @param[in,out] stream Output stream to be printed to
   * @return The given stream
   * @note This function is not useful when the command is not interpreted yet.
   */
  std::ostream &print(std::ostream &stream) {
    stream << "Code: " << static_cast<int>(code_) << std::endl;
    stream << "Argc: " << static_cast<int>(argc_) << std::endl;
    stream << "Argv: " << std::endl;
    int sz = argc_;
    if (argc_ > 10) {
      sz = 10;
    }
    for (int i = 0; i < sz; i++) {
      stream << static_cast<int>(arguments_[i]) << " ";
    }
    stream << std::endl;
    return stream;
  }

private:
  std::vector<uint8_t> command_;
  uint16_t code_ = 0;
  uint16_t argc_ = 0;
  std::vector<uint32_t> arguments_;
  bool updated_ = false;
};
template <typename T>
inline bool CommunicationFormat::checkHeaderFooter(const T &v) {
  const int n = v.size();
  if (n < 10) {
    std::cerr << "Command is too short!!: length = " << n << std::endl;
    return false;
  }

  if (static_cast<uint8_t>(v[0]) != 0xeb || static_cast<uint8_t>(v[1]) != 0x90 || static_cast<uint8_t>(v[2]) != 0x5b || static_cast<uint8_t>(v[3]) != 0x6a) {
    std::cerr << "start code incorect" << std::endl;
    return false;
  }
  if (static_cast<uint8_t>(v[n - 4]) != 0xc5 || static_cast<uint8_t>(v[n - 3]) != 0xa4 || static_cast<uint8_t>(v[n - 2]) != 0xd2 || static_cast<uint8_t>(v[n - 1]) != 0x79) {
    std::cerr << "stop code incorrect" << std::endl;
    return false;
  }
  return true;
}

template <typename T>
bool CommunicationFormat::validate(const T &v, uint16_t argc) {
  const int n = v.size();
  if (n != 14 + 4 * static_cast<int>(argc)) {
    std::cerr << "Invalid command: length not appropriate" << std::endl;
    std::cerr << "The length of command should be " << 14 + 4 * static_cast<int>(argc) << ", but now it is " << n << std::endl;
    return false;
  }

  std::vector<uint8_t> com_without_fotter;
  for (int i = 0; i < n - 6; i++) {
    com_without_fotter.push_back(v[i]);
  }
  uint16_t crc_calc = calcCRC16(com_without_fotter);
  uint16_t crc_attached = getValue<uint16_t>(n - 6);
  if (crc_calc != crc_attached) {
    std::cerr << "Invalid command: CRC16 not appropriate" << std::endl;
    return false;
  }
  return true;
}
} // namespace gramsballoon::pgrams

#endif /* CommandDefinition_H */
