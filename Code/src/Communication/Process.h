#ifndef PROCESS_HEADER_THOMAS_CHEVALIER
#define PROCESS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Process data received from the host
 * @details Executes commands if needed. May take a long time to return.
 * 
 * @param data The data received from the host
 * @param lenght The length of the byte array
 */
void com_process_data(uint8_t* data, uint8_t lenght);

#endif // PROCESS_HEADER_THOMAS_CHEVALIER
