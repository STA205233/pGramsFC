#ifndef CommandSaver_HH
#define CommandSaver_HH 1

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

namespace gramsballoon
{

void write_command(const std::vector<uint8_t>& command, const std::string& command_name);
std::string get_time_stamp_str();
std::string get_log_filename();

} /*namespace gramsballoon*/

#endif
