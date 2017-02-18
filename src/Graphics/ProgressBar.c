#include "ProgressBar.h"

#include "../Globals.h"
#include "Drawing.h"


void progress_begin(uint8_t complexity)
{
	PROGRESS_COMPLEXITY = complexity;
	PROGRESS_ADVANCE = 0;

	// Frame of progress bar
	draw_h_line(14, 56, 100, WHITE);
	draw_h_line(14, 61, 100, WHITE);
	draw_v_line(14, 57, 4, WHITE);
	draw_v_line(113, 57, 4, WHITE);
	draw_update();
}

void progress_add(uint8_t work)
{
	if(PROGRESS_COMPLEXITY == 0)
	{
		return;
	}

	uint16_t adv  = PROGRESS_ADVANCE;
	const uint16_t cplx = PROGRESS_COMPLEXITY;

	const uint8_t minimum_pix = adv * 100 / cplx;
	PROGRESS_ADVANCE += work;
	adv = PROGRESS_ADVANCE;
	const uint8_t maximum_pix = adv * 100 / cplx;

	for(uint8_t i = 0; i < 4; ++i)
	{
		draw_h_line(14+minimum_pix, 57+i, maximum_pix-minimum_pix, WHITE);
	}
	
	draw_update();
}

void progress_end(void)
{
	PROGRESS_COMPLEXITY = 0;
	PROGRESS_ADVANCE = 0;
}