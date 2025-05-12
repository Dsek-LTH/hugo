#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "hugo.h"

#define LEVELS 10
#define TICK_FRAMES 20

#define INDEX(x,y) (HUGO_WIDTH*y + x)

int main() {
	hugo_setup();
	
	int radar_front = 0;
	int dir = 1;

	int image[HUGO_WIDTH * HUGO_HEIGHT];
	for (int i=0; i<HUGO_WIDTH*HUGO_HEIGHT; ++i) {
		image[i] = 0;
	}

	// draw yao!
	int cycle_index = 0;
	int frame_counter = 0;
	while(true) {
		for (int y=0; y<HUGO_HEIGHT; ++y) {
			hugo_begin_row(y);
			for (int x=0; x<HUGO_WIDTH; ++x) {
				//hugo_draw((x % LEVELS) <= cycle_index);
				hugo_draw((image[INDEX(x,y)]) > cycle_index);
			}
			hugo_end_row(y);
		}
		cycle_index = (cycle_index + 1) % LEVELS;

		if (frame_counter == 0) {
			frame_counter = TICK_FRAMES;

			for (int i=0; i<HUGO_WIDTH*HUGO_HEIGHT; ++i) {
				if (image[i] > 0) --image[i];
			}

			for (int y=0; y<HUGO_HEIGHT; ++y) {
				image[INDEX(radar_front, y)] = LEVELS;

				if (rand() < 10000000) image[INDEX(radar_front, y)] = 60;
			}

			radar_front += dir;
			if (radar_front == HUGO_WIDTH-1 || radar_front == 0) {
				dir = -dir;
			}
		}
		else {
			--frame_counter;
		}
	}
}


