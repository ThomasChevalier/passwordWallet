#ifndef PROGRESSBAR_HEADER_THOMAS_CHEVALIER
#define PROGRESSBAR_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

/**
 * @brief Initialize the progress bar
 * @details An empty frame is drawn on the screen.
 * 
 * @param complexity The number of step that the process will take
 */
void progress_begin(uint8_t complexity);

/**
 * @brief Pause the progress bar.
 * @details Any call to progress_add will be without effect.
 */
void progress_pause(void);

/**
 * @brief Unpause the progress bar.
 */
void progress_continue(void);

/**
 * @brief Add steps to the progress bar.
 * @details It will update the screen to show the progress advance.
 * 
 * @param work The number of step to add to the progress counter.
 * Be sure to not add more steps than the defined complexity.
 */
void progress_add(uint8_t work);


/**
 * @brief Reset complexity and advancement to 0.
 */
void progress_end(void);

#endif // PROGRESSBAR_HEADER_THOMAS_CHEVALIER
