#ifndef _BRICK_H
#define _BRICK_H

#include "ctris.h"

void change_direction(char [BOARD_HEIGHT][BOARD_WIDTH], char [4][4], const unsigned char, const unsigned char, const char);
void find_index(char [4][4], unsigned char *, unsigned char *);
char check_brick(char [BOARD_HEIGHT][BOARD_WIDTH], char [4][4], const unsigned char, const unsigned char);
void draw_to_board(char [BOARD_HEIGHT][BOARD_WIDTH], char [4][4], const char, const unsigned char, const unsigned char);

char brick_digit[7][4][4];

#endif

