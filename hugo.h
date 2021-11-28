#pragma once

#include <stdbool.h>

#define HUGO_WIDTH 95
#define HUGO_HEIGHT 7

void hugo_setup();
void hugo_set_busy_wait(int loops);
void hugo_begin_row(int row);
void hugo_end_row(int row);
void hugo_draw(bool pixel);

