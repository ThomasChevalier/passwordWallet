#ifndef STATES_HEADER_THOMAS_CHEVALIER
#define STATES_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// STATES

typedef enum {
	STATE_INIT = 0,
	STATE_MAIN,
	STATE_BROWSE,
	STATE_OPTION,
	STATE_SAVE,
	// ...
	NUM_STATES
} State_type;

typedef struct State_
{
	uint8_t event_mask;			// Each bit of the event_mask is an event, a bit set to 1 mean the state react to the specified event
	uint8_t (*transition)(uint8_t);	// Transition to the next state, return new state number
} State;


#endif // STATES_HEADER_THOMAS_CHEVALIER
