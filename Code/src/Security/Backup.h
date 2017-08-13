#ifndef BACKUP_HEADER_THOMAS_CHEVALIER
#define BACKUP_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#define BACKUP_STATUS_OK (0)
#define BACKUP_STATUS_UPDATE (1)
#define BACKUP_STATUS_CHANGE_KEY (2)
#define BACKUP_STATUS_CHANGE_KEY_CHUNK_OK (3)
#define BACKUP_STATUS_CHANGE_KEY_UPDATE_CARD (4)
#define BACKUP_STATUS_CHANGE_KEY_VALIDATION (5)


/**
 * @brief Makes a backup of the specified password chunk.
 * @details This backup is useful is something goes wrong when updating a password chunk.
 * 
 * @param pwd_id The id of the password chunk to backup.
 * @param statut The statuts of the current operation.
 */
void backup_save (uint8_t pwd_id, uint8_t statuts);

/**
 * @brief Restores the password chunk from the backup section.
 * @details Warning : this will overwrite the password chunk.
 * The id of the chunk restored is the id stored in the backup section.
 * 
 */
void backup_restore (void);

/**
 * @brief Free the backup section.
 * @details Mark operation as completed.
 */
void backup_free (void);

/**
 * @brief Set the status of the operation.
 */
void backup_set_status(uint8_t status);

/**
 * @brief Check if something goes wrong the last time the device was power one.
 * @details Call this function at the startup of the application. It will try to recover
 * broken data due to an incomplete operation. This can happen when the device is unplug when
 * change_key() is called for example, or when a password is changed.
 */
void backup_recover(void);

#endif // BACKUP_HEADER_THOMAS_CHEVALIER
