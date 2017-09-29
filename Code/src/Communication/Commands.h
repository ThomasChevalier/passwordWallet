#ifndef COMMANDS_HEADER_THOMAS_CHEVALIER
#define COMMANDS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

#define COM_OK        (0x00)
#define COM_ERR_NOT_INIT (0x01)
#define COM_INIT      (0x02)
#define COM_END       (0x03)

#define COM_GET_FRAM  (0x10)
#define COM_GET_KEY   (0x11)
#define COM_GET_PARAM (0x12)

#define COM_SET_FRAM  (0x20)
#define COM_SET_KEY   (0x21)

#define COM_FRAM (0x30)
#define COM_KEY (0x31)
#define COM_PARAM (0x32)

typedef struct Command_
{
	uint8_t id;
	uint16_t totalSize;
	uint16_t availableSize;
	uint8_t *data;
} Command;

extern Command CURRENT_COMMAND;

void com_exec(uint8_t id);
void com_abort(void);

void command_init(void);
void command_end(void);

void command_get_fram(void);
void command_get_key(void);
void command_get_param(void);

void command_set_fram(void);
void command_set_key(void);

#endif // COMMANDS_HEADER_THOMAS_CHEVALIER