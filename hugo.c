#include <wiringPi.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define WIDTH HUGO_WIDTH
#define HEIGHT HUGO_HEIGHT

#define SHIFT 23
#define CLEAR 24
#define COLUMN 25

#define DEFAULT_BUSY_WAIT 20000
#define DEFAULT_CLOCK_WAIT 100

const int rows[] = {2, 3, 12, 13, 14, 22, 21};
const int nrows = sizeof(rows) / sizeof(int);

int busy_wait_loops = DEFAULT_BUSY_WAIT;
int clock_wait_loops = DEFAULT_CLOCK_WAIT;


static void clock_wait() {
	volatile int n=clock_wait_loops;
	while(n --> 0);
}

static void shift() {
	digitalWrite(SHIFT, HIGH);
	clock_wait();
	digitalWrite(SHIFT, LOW);
}

static void clear() {
	digitalWrite(CLEAR, LOW);
	clock_wait();
	digitalWrite(CLEAR, HIGH);
}

static void enable_row(int i) {
	digitalWrite(rows[i], HIGH);
}

static void disable_row(int i) {
	digitalWrite(rows[i], LOW);
}

void hugo_set_busy_wait(int loops) {
	busy_wait_loops = loops;
}

void hugo_setup() {
	char* value;
	if ((value = getenv("HUGO_BUSY_WAIT")) != NULL) busy_wait_loops = atoi(value);
	if ((value = getenv("HUGO_CLOCK_WAIT")) != NULL) clock_wait_loops = atoi(value);

	wiringPiSetup();

	pinMode(SHIFT, OUTPUT);
	digitalWrite(SHIFT, LOW);

	pinMode(COLUMN, OUTPUT);
	digitalWrite(SHIFT, LOW);

	pinMode(CLEAR, OUTPUT);
	digitalWrite(SHIFT, HIGH);

	for (int i=0; i<nrows; ++i) {
		pinMode(rows[i], OUTPUT);
		digitalWrite(rows[i], LOW);
	}
}

void hugo_begin_row(int i) {
}

void hugo_end_row(int i) {
	enable_row(i);
	volatile int n=busy_wait_loops;
	while (n --> 0);
	disable_row(i);
	clear();
}

void hugo_draw(bool pixel) {
	digitalWrite(COLUMN, pixel ? HIGH : LOW);
	shift();
}

