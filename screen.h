#ifndef _SCREEN_H
#define _SCREEN_H

#include "ctris.h"

void restore_screen();
char init_screen();
WINDOW *create_win(const char, const char, const char, const char);
WINDOW *create_board_win();
WINDOW *create_preview_win();
WINDOW *create_score_win();
void destroy_board_win(WINDOW *);
void destroy_preview_win(WINDOW *);
void destroy_score_win(WINDOW *);
void show_brick_pice(WINDOW *, const char, const unsigned char, const unsigned char);
void show_board_win(WINDOW *, char [BOARD_HEIGHT][BOARD_WIDTH], char [4][4], const char, const unsigned char, const unsigned char);
void show_brick(WINDOW *, char [4][4], const char, const unsigned char, const unsigned char);
void show_brick_preview(WINDOW *, const char);
int get_key(WINDOW *);
int old_get_key(WINDOW *);
void show_game_over(WINDOW *);
void show_score(WINDOW *, const unsigned int, const char);
void show_highscore(const char *);
void read_string(const char *, const char *, char *, const unsigned int);
void add_user_to_highscore(char *, const unsigned int);
void wait_for_start(WINDOW *);
void show_pause(WINDOW *);
void show_headline();
void show_colorized_char(const unsigned char, const unsigned char, const char, const char);
void show_remove_row(WINDOW *, char [BOARD_HEIGHT][BOARD_WIDTH], const unsigned char);
void refresh_win(WINDOW *);
void show_yes_no(WINDOW *, const char);
char yes_no_question(const char *);
char play_again();

#endif


