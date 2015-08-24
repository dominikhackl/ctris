#ifndef _CTRIS_H
#define _CTRIS_H

#include "config.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
#include <time.h>

#define VERSION "v0.42" // version info
#define HEIGHT 24 // height of the screen
#define WIDTH 80 // width of the screen
#define BOARD_HEIGHT (HEIGHT - 5) // height of the board
#define BOARD_WIDTH 15 // width of the board
#define EXTRA_BONUS // if defined, you get a bonus for every brick
#define YES 0
#define NO 1
#define REMOVE_SPLASH_TIME 500000 // (microseconds)

//#define HIDE_CURSOR_WORKAROUND // only needed if autodetection doesn't work

// with this variables you can change the average constancy of a game
#define SPEED_CONST_1 500
#define SPEED_CONST_2 15000
#define BONUS_CONST 100
#define LEVEL_CONST 300

void handle_quit_signal(const int);
unsigned char get_rand(const unsigned char);
int main(int, char *[]);

char quit, old_style_keys, counterclockwise_rotation, hide_cursor_workaround, highscore_file_path[256], default_name[40];

unsigned long rseed;

#endif


