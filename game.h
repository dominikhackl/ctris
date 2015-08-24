#ifndef _GAME_H
#define _GAME_H

#include "ctris.h"

void init_board(char [BOARD_HEIGHT][BOARD_WIDTH]);
void remove_this_row(WINDOW *, char [BOARD_HEIGHT][BOARD_WIDTH], unsigned char);
void remove_rows(WINDOW *, char [BOARD_HEIGHT][BOARD_WIDTH], unsigned int *, const char);
void calc_level(const unsigned int, char *);
unsigned int start_game();


#endif

