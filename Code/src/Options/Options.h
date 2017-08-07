#ifndef OPTIONS_HEADER_THOMAS_CHEVALIER
#define OPTIONS_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

typedef struct _Option_Node
{
	void (*callback)(void);  //< Function to call when a final node is selected, NULL if their is children, not NULL otherwise
	uint8_t child;
	uint8_t neighbour;
	uint8_t str;
} Option_Node;

#define OPT_PWD (0)

void options_display(uint8_t opt);

#endif // AES_HEADER_THOMAS_CHEVALIER
