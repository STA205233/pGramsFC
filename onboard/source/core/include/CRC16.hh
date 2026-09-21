#ifndef CRC16_H
#define CRC16_H 1

#include <cstdint>
#include <iostream>
#include <vector>

namespace gramsballoon {

/**
 * @brief Calculate the CRC-16 checksum of the given data
 * @param[in] pbuffer Data whose checksum is calculated
 */
uint16_t calcCRC16(const std::vector<uint8_t> &pbuffer);

} /* namespace gramsballoon */

#endif /* CRC16_H */
