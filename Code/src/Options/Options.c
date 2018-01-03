#include "Options.h"

#include <avr/pgmspace.h>

#include "../Graphics/String.h"
#include "../Graphics/Drawing.h"
#include "../Program/Program.h"
#include "../FSM/Events.h"

#include "../Hardware/Buttons.h"

/**
 * @brief Draw a maximum of 3 neighbour and return the number of node
 * 
 * @param node The node to draw
 * @return The number of nodes
 */

static uint8_t draw_nodes(const Option_Node* node)
{
	uint8_t numOpt = 0;
	draw_clear();
	for(;;)
	{
		if(numOpt < 3)
		{
			draw_flash_string(2, numOpt*23+2, node->str);
		}
		
		++numOpt;

		if(node->neighbour == OPTIONS_LIST_NONE)
		{
			break;
		}
		node = &list_options[node->neighbour];
	}
	return numOpt;
}

/**
 * @brief Draw the selection rectangle
 * 
 * @param current The current position
 * @param max The maximum value that current can be + 1
 */
static void draw_selection(uint8_t current, uint8_t max)
{
	uint8_t l_table[3] = {0, 20, 43};
	uint8_t pos = 0;
	if(current == 0){
		pos = 0;
	}
	else if(current != max-1)
	{
		pos = 1;
	}
	else
	{
		pos = 2;
	}
	uint8_t i;
	for(i = 0; i < 21; ++i)
	{
		draw_h_line(0, l_table[pos] + i, 128, INVERSE);
	}
}

/**
 * @brief Return a specific node from the base node.
 * @details If the position given to the function is too big then the function return the last element of the list.
 * 
 * @param base The node to search from.
 * @param pos The position of the wanted node.
 * 
 * @return The node at the position "pos"
 */
static const Option_Node* node_at(const Option_Node* base, uint8_t pos)
{
	while(base->neighbour != OPTIONS_LIST_NONE && pos)
	{
		--pos;
		base = &list_options[base->neighbour];
	}
	return base;
}

void options_display(uint8_t opt)
{
	const Option_Node* base = &list_options[opt];

	uint8_t current_choice = 0;
	uint8_t maxChoice = draw_nodes(base);

	draw_selection(current_choice, maxChoice);

	draw_update();


	while(buttons_pressed());

	const Option_Node* nodeStack[OPTIONS_LIST_MAX_INTRICATION];
	uint8_t nodeStackPos = 0;

	for(;;)
	{
		program_update();
		uint8_t events = events_get();
		if(events & EVENT_BUTTON_1)
		{
			if(current_choice == 0) {
				current_choice = maxChoice-1;
			} else {
				--current_choice;
			}
		}
		else if(events & EVENT_BUTTON_2)
		{
			const Option_Node* nd = node_at(base, current_choice);
			if(nd->callback != NULL)
			{
				nd->callback();
				return;
			}
			else if(nd->child != OPTIONS_LIST_NONE)
			{
				current_choice = 0;
				nodeStack[nodeStackPos++] = base;
				base = &list_options[nd->child];
				maxChoice = draw_nodes(base);
				draw_selection(current_choice, maxChoice);
				draw_update();
			}
		}
		else if(events & EVENT_BUTTON_3)
		{
			++current_choice;
			if(current_choice == maxChoice) {
				current_choice = 0;
			}
		}
		else if(events & EVENT_BUTTON_4)
		{
			if(nodeStackPos == 0){
				return;
			} else {
				base = nodeStack[--nodeStackPos];
				current_choice = 0;
				maxChoice = draw_nodes(base);
				draw_selection(current_choice, maxChoice);
				draw_update();
			}
		}

		// If an event happen redraw the menu
		// But if this is button 2, then or the
		// function has return or a child has been
		// selected and the menu drawn.
		if(events && !(events & EVENT_BUTTON_2))
		{
			if((current_choice == 0 ) || (maxChoice <= 3))
			{
				draw_nodes(base);
			}
			else if(current_choice == maxChoice-1)
			{
				draw_nodes( node_at(base, current_choice - 2));
			}
			else
			{
				draw_nodes( node_at(base, current_choice - 1) );
			}
			draw_selection(current_choice, maxChoice);
			draw_update();
		}

		program_wait();
	}
}
