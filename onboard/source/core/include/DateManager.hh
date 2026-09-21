#ifndef DateManager_H
#define DateManager_H 1

#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <sys/time.h>

namespace gramsballoon {

/**
 * @brief Return the current local time as a string
 * @return Time string in the YYYYMMDDHHMMSS format
 */
std::string getTimeStr();

} /* namespace gramsballoon */

#endif /* DateManager_H */
