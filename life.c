#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "hugo.h"

#define WIDTH HUGO_WIDTH
#define HEIGHT HUGO_HEIGHT


// --- crc for loop check ---
uint32_t table[256];
void crc_init() {
	for(int i=0; i<256; ++i) {
		table[i] = rand();
	}
}

uint32_t crc(uint8_t* data, size_t bytes) {
	uint32_t checksum = 0xffffffff;
	for(int i=0; i<bytes; ++i) {
		int index = (checksum ^ data[i]) & 0xff;
		checksum = (checksum >> 8) ^ table[index];
	}
	return checksum;
}

// --- game below ---

int image[WIDTH * HEIGHT];
int buffer[WIDTH * HEIGHT];

#define MAX_LOOP_LENGTH 100
uint32_t last_checksums[MAX_LOOP_LENGTH];
int last_head = 0;

#define INDEX(x,y) (y*WIDTH+x)

int get_cell(int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;
	else return image[INDEX(x,y)];
}

int neighbors(int x, int y) {
	return (get_cell(x-1, y-1) ? 1 : 0)
	     + (get_cell(x  , y-1) ? 1 : 0)
	     + (get_cell(x+1, y-1) ? 1 : 0)
	     + (get_cell(x-1, y  ) ? 1 : 0)
	     + (get_cell(x+1, y  ) ? 1 : 0)
	     + (get_cell(x-1, y+1) ? 1 : 0)
	     + (get_cell(x  , y+1) ? 1 : 0)
	     + (get_cell(x+1, y+1) ? 1 : 0);
}

void evolve() {
	for (int y=0; y<HEIGHT; ++y) {
		for (int x=0; x<WIDTH; ++x) {
			int i = INDEX(x,y);
			int n = neighbors(x, y);
			if (image[i]) {
				if (n < 2) buffer[i] = false;
				else if (n > 3) buffer[i] = false;
				else buffer[i] = true;
			}
			else if (n == 3) buffer[i] = true;
			else buffer[i] = false;
		}
	}
	memmove(image, buffer, sizeof(buffer));
}

void initialize() {
	// initialize image
	for (int i=0; i<WIDTH*HEIGHT; ++i) {
		image[i] = rand() % 2;
	}
}

bool detect_loop() {
	uint32_t checksum = crc((uint8_t*)image, sizeof(image));
	for (int i=0; i < MAX_LOOP_LENGTH; ++i) {
		if (checksum == last_checksums[i]) return true;
	}
	last_checksums[last_head] = checksum;
	last_head = (last_head + 1) % MAX_LOOP_LENGTH;
	return false;
}

#define FRAMES_PER_GENERATION 50
#define PAUSE_FRAMES 1000

int main() {
	hugo_setup();

	srand(time(NULL));
	crc_init();
	initialize();

	// draw yao!
	int frame_counter = FRAMES_PER_GENERATION;
	int restart_counter = 0; //PAUSE_FRAMES;
	while(true) {
		for (int y=0; y<HEIGHT; ++y) {
			hugo_begin_row(y);
			for (int x=0; x<WIDTH; ++x) {
				hugo_draw(image[WIDTH * y + x]);
			}
			hugo_end_row(y);
		}

		if (restart_counter == 0) {
			if (--frame_counter == 0) {
				evolve();
				if (detect_loop()) {
					restart_counter = PAUSE_FRAMES;
				}

				frame_counter = FRAMES_PER_GENERATION;
			}
		} else {
			if (--restart_counter == 0) {
				initialize();
			}
		}
	}
}

