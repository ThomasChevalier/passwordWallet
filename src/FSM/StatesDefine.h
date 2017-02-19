#ifndef STATES_DEFINE_HEADER_THOMAS_CHEVALIER
#define STATES_DEFINE_HEADER_THOMAS_CHEVALIER

#include <stdint.h>

// STATES
typedef enum {
	STATE_WAIT_CARD = 0,
	STATE_RECOVERY,
	STATE_MAIN,
	STATE_BROWSE,
	STATE_OPTION,
	STATE_OPTION_PASSWORD,
	STATE_OPTION_SORT,
	STATE_OPTION_ADVANCED,
	// ...
	NUM_STATES
} State_type;

typedef struct State_
{
	// uint8_t event_mask;			// Each bit of the event_mask is an event, a bit set to 1 mean the state react to the specified event
	uint8_t (*transition)(uint8_t);	// Transition to the next state, return new state number
	void (*begin)(void); // The "constructor" of the state, called when the state is entered
	void (*end)(void); // The "destructor" of the state, called when the state is left
} State;

#endif // STATES_DEFINE_HEADER_THOMAS_CHEVALIER
