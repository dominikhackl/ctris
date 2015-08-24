#include "ctris.h"
#include "screen.h"
#include "brick.h"
#include "colors.h"
#include "highscore.h"

// workaround for the FreeBSD 4 console to let the cursor disapear
void hide_cursor(WINDOW *win)
{
	if(hide_cursor_workaround == 1)
	{
		mvwprintw(win, 0, 0, "");
	}
}

void restore_screen()
{
	clear();
	curs_set(1);
	endwin();
}

char init_screen()
{
	int x, y;
	initscr();
	if(has_colors() != TRUE)
	{
		endwin();
		printf("ERROR, your terminal can't display colors.\n");
		return 1;
	}
	getmaxyx(stdscr, y, x);
	if(x < WIDTH || y < HEIGHT)
	{
		endwin();
		printf("ERROR, your terminal is too small. (min %ux%u)\n", WIDTH, HEIGHT);
		return 1;
	}
	clear();
	noecho();
	
	start_color();
	
	init_pair(COLOR_BLACK_BG, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_GREEN_BG, COLOR_WHITE, COLOR_GREEN);
	init_pair(COLOR_WHITE_BG, COLOR_WHITE, COLOR_WHITE);
	init_pair(COLOR_RED_BG, COLOR_WHITE, COLOR_RED);
	init_pair(COLOR_CYAN_BG, COLOR_WHITE, COLOR_CYAN);
	init_pair(COLOR_MAGENTA_BG, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(COLOR_BLUE_BG, COLOR_WHITE, COLOR_BLUE);
	init_pair(COLOR_YELLOW_BG, COLOR_WHITE, COLOR_YELLOW);

	init_pair(COLOR_BLACK_FG, COLOR_BLACK, COLOR_BLACK);
	init_pair(COLOR_GREEN_FG, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_WHITE_FG, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_RED_FG, COLOR_RED, COLOR_BLACK);
	init_pair(COLOR_CYAN_FG, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_MAGENTA_FG, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_BLUE_FG, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_YELLOW_FG, COLOR_YELLOW, COLOR_BLACK);

	if(curs_set(0) == ERR)
	{
		hide_cursor_workaround = 1;
	}
	return 0;
}

WINDOW *create_win(const char b, const char a, const char y, const char x)
{
	WINDOW *win;
	win = newwin((int)b, (int)a, (int)y, (int)x);
	refresh();
	box(win, 0, 0);
	refresh_win(win);
	return win;
}

WINDOW *create_board_win()
{
	WINDOW *win;
	win = create_win(BOARD_HEIGHT + 2, BOARD_WIDTH * 2 + 2, 3, 0);
	keypad(win, TRUE);
	wtimeout(win, 0);
	return win;
}

void destroy_board_win(WINDOW *win)
{
	delwin(win);
}

WINDOW *create_preview_win()
{
	WINDOW *win;
	win = create_win(6, 12, 3, BOARD_WIDTH * 2 + 2);
	return win;
}

void destroy_preview_win(WINDOW *win)
{
	delwin(win);
}

WINDOW *create_score_win()
{
	WINDOW *win;
	win = create_win(12, 12, 9, BOARD_WIDTH * 2 + 2);
	return win;
}

void destroy_score_win(WINDOW *win)
{
	delwin(win);
}

void show_brick_pice(WINDOW *win, const char brick_type, const unsigned char x, const unsigned char y)
{
	int color;
	switch(brick_type)
	{
		case 1:
			color = COLOR_RED_BG;
			break;
		case 2:
			color = COLOR_YELLOW_BG;
			break;
		case 3:
			color = COLOR_BLUE_BG;
			break;
		case 4:
			color = COLOR_GREEN_BG;
			break;
		case 5:
			color = COLOR_MAGENTA_BG;
			break;
		case 6:
			color = COLOR_WHITE_BG;
			break;
		case 7:
			color = COLOR_CYAN_BG;
			break;
		default:
			color = 0;
			break;
	}
	if(color != 0)
	{
		wattron(win, COLOR_PAIR(color));
	}
	mvwprintw(win, (int)y + 1, (int)x * 2 + 1, "  ");
	if(color != 0)
	{
		wattroff(win, COLOR_PAIR(color));
	}
}

void show_board_win(WINDOW *win, char board[BOARD_HEIGHT][BOARD_WIDTH], char cur_brick[4][4], const char brick_type, const unsigned char x, const unsigned char y)
{
	unsigned char i, n;
	for(i = 0; i < BOARD_HEIGHT; i++)
	{
		for(n = 0; n < BOARD_WIDTH; n++)
		{
			show_brick_pice(win, board[i][n], n, i);
		}
	}
	show_brick(win, cur_brick, brick_type, x, y);
	refresh_win(win);
}

void show_brick(WINDOW *win, char cur_brick[4][4], const char brick_type, const unsigned char x, const unsigned char y)
{
	unsigned char i, n, a, b;
	find_index(cur_brick, &a, &b);
	for(i = 0; i < 4; i++)
	{
		for(n = 0; n < 4; n++)
		{
			if(cur_brick[i][n] != 0 && x - a + n >= 0 && y - b + i >= 0)
			{
				show_brick_pice(win, brick_type, x - a + n, y - b + i);
			}
		}
	}
	refresh_win(win);
}

void show_brick_preview(WINDOW *win, const char brick_type)
{
	unsigned char x, y;
	for(y = 0; y < 4; y++)
	{
		for(x = 1; x < 5; x++)
		{
			show_brick_pice(win, 0, x, y);
		}
	}
	show_brick(win, brick_digit[brick_type - 1], brick_type, 2, 1);
}

int get_key(WINDOW *win)
{
	return wgetch(win);
}

int old_get_key(WINDOW *win)
{
	int retval;
	notimeout(win, TRUE);
	retval = get_key(win);
	notimeout(win, FALSE);
	return retval;
}

void show_game_over(WINDOW *win)
{
	wattron(win, A_BOLD);
	mvwprintw(win, (BOARD_HEIGHT / 2) - 1, BOARD_WIDTH - 8, "G A M E   O V E R");
	wattroff(win, A_BOLD);
	mvwprintw(win, (BOARD_HEIGHT / 2) + 1, BOARD_WIDTH - 11 , "Press a key to continue!");
	refresh_win(win);
	old_get_key(win);
}

void show_score(WINDOW *win, const unsigned int score, const char level)
{
	const int highlight_color = COLOR_RED_FG;
	mvwprintw(win, 2, 2, "Score:");
	mvwprintw(win, 7, 2, "Level:");
	wattron(win, COLOR_PAIR(highlight_color));
	mvwprintw(win, 4, 2, "%u", score);
	mvwprintw(win, 9, 2, "%u", (unsigned int)level);
	wattroff(win, COLOR_PAIR(highlight_color));
	refresh_win(win);
}

void wait_for_start(WINDOW *win)
{
	mvwprintw(win, (BOARD_HEIGHT / 2), BOARD_WIDTH - 10 , "Press a key to start!");
	refresh_win(win);
	old_get_key(win);
}

void show_highscore(const char *name)
{
	WINDOW *win;
	unsigned char i, n;
	char tmp_string[10], colorized_row = 0;
	struct highscore_struct highscore;
	const int highlight_color = COLOR_RED_FG;
	read_highscore(&highscore);
	clear();
	win = create_win(16, 43, (HEIGHT / 2) - 8, (WIDTH / 2) - 21);
	keypad(win, TRUE);
	mvwprintw(win, 2, 5, "Highscore:");
	for(i = 0; i < 10; i++)
	{
		if(strncmp(name, highscore.entry[i].name, 40) == 0)
		{
			wattron(win, COLOR_PAIR(highlight_color));
			colorized_row = 1;
		}
		highscore.entry[i].name[20] = '\0';
		for(n = 0; n < 20; n++)
		{
			mvwprintw(win, (int)4 + i, (int)9 + n, ".");
		}
		mvwprintw(win, (int)4 + i, 5, "%2u. %s", (unsigned int)i + 1, highscore.entry[i].name);
		mvwprintw(win, (int)4 + i, 28, "%9u", highscore.entry[i].score);
		snprintf(tmp_string, 9, "%u", highscore.entry[i].score);
		for(n = 0; n < 9 - strlen(tmp_string); n++)
		{
			mvwprintw(win, (int)4 + i, (int)28 + n, ".");
		}
		if(colorized_row != 0)
		{
			wattroff(win, COLOR_PAIR(highlight_color));
			colorized_row = 0;
		}
	}
	refresh_win(win);
	delwin(win);
	getch();
}

void read_string(const char *headline, const char *default_string, char *string, const unsigned int size)
{
	WINDOW *win;
	clear();
	win = create_win(7, 60, (HEIGHT / 2) - 4, (WIDTH / 2) - 30);
	keypad(win, TRUE);
	echo();
	curs_set(1);
	mvwprintw(win, 2, 5, "%s (default: %s)", headline, default_name);
	refresh_win(win);
	if(default_name[0] != '\0')
	{
		if(mvwscanw(win, 4, 5, "%s", string) != 1)
		{
			strncpy(string, default_string, size);
		}
	}
	else
	{
		while(mvwscanw(win, 4, 5, "%s", string) == ERR);
	}
	noecho();
	curs_set(0);
	delwin(win);
}

void add_user_to_highscore(char *name, const unsigned int score)
{
	read_string("Please enter your name:", default_name, name, 40);
	strncpy(default_name, name, 40);
	add_to_highscore(name, score);
}

void show_pause(WINDOW *win)
{
	wattron(win, A_BOLD);
	mvwprintw(win, (BOARD_HEIGHT / 2) - 1, BOARD_WIDTH - 4 , "P A U S E");
	wattroff(win, A_BOLD);
	mvwprintw(win, (BOARD_HEIGHT / 2) + 1, BOARD_WIDTH - 10, "Press 'p' to continue!");
	refresh_win(win);
	while(old_get_key(win) != 'p');
}

void show_colorized_char(const unsigned char x, const unsigned char y, const char color, const char c)
{
	attron(COLOR_PAIR((int)color));
	mvprintw((int)y, (int)x, "%c", c);
	attroff(COLOR_PAIR((int)color));
}

void show_headline()
{
	clear();
	
	//show_colorized_char((((BOARD_WIDTH * 2) + 10) / 2) - 5, 1, COLOR_RED_FG, 'C');
	show_colorized_char((((BOARD_WIDTH * 2) + 10) / 2) - 5, 1, COLOR_RED_FG, 'C');
	show_colorized_char((((BOARD_WIDTH * 2) + 10) / 2) - 5 + 2, 1, COLOR_MAGENTA_FG, 'T');
	show_colorized_char((((BOARD_WIDTH * 2) + 10) / 2) - 5 + 4, 1, COLOR_YELLOW_FG, 'R');
	show_colorized_char((((BOARD_WIDTH * 2) + 10) / 2) - 5 + 6, 1, COLOR_WHITE_FG, 'I');
	show_colorized_char((((BOARD_WIDTH * 2) + 10) / 2) - 5 + 8, 1, COLOR_GREEN_FG, 'S');
	
	mvprintw(HEIGHT - 1, (BOARD_WIDTH * 2) + 9, VERSION);

	refresh();
}

void show_remove_row(WINDOW *win, char board[BOARD_HEIGHT][BOARD_WIDTH], const unsigned char y)
{
	unsigned char i;
	wattron(win, A_REVERSE);
	for(i = 0; i < BOARD_WIDTH; i++)
	{
		show_brick_pice(win, board[y][i], i, y);
	}
	wattroff(win, A_REVERSE);
}

void refresh_win(WINDOW *win)
{
	hide_cursor(win);
	wrefresh(win);
}

void show_yes_no(WINDOW *win, const char cur_marked)
{
	const int highlight_color = COLOR_GREEN_BG;
	if(cur_marked == YES)
	{
		wattron(win, COLOR_PAIR(highlight_color));
	}
	mvwprintw(win, 4, 10, "[ YES ]");
	if(cur_marked == YES)
	{
		wattroff(win, COLOR_PAIR(highlight_color));
	}
	else if(cur_marked == NO)
	{
		wattron(win, COLOR_PAIR(highlight_color));
	}
	mvwprintw(win, 4, 24, "[ NO ]");
	if(cur_marked == NO)
	{
		wattroff(win, COLOR_PAIR(highlight_color));
	}
	refresh_win(win);
	
}

char yes_no_question(const char *question)
{
	WINDOW *win;
	char cur_marked = YES, run = 0;
	clear();
	win = create_win(7, 40, (HEIGHT / 2) - 4, (WIDTH / 2) - 20);
	keypad(win, TRUE);
	mvwprintw(win, 2, 20 - (strlen(question) / 2), question);
	show_yes_no(win, cur_marked);
	while(run == 0)
	{
		switch(get_key(win))
		{
			case 'y':
				cur_marked = YES;
				run = 1;
				break;
			case 'q':
			case 'n':
				cur_marked = NO;
				run = 1;
				break;
			case KEY_LEFT:
				cur_marked = YES;
				break;
			case KEY_RIGHT:
				cur_marked = NO;
				break;
			case 9:
				if(cur_marked == YES)
				{
					cur_marked = NO;
				}
				else
				{
					cur_marked = YES;
				}
				break;
			case ' ':
			case 10:
				run = 1;
				break;
		}
		show_yes_no(win, cur_marked);
	}
	delwin(win);
	return cur_marked;
}

char play_again()
{
	if(yes_no_question("Do you want to play again?") == YES)
	{
		return 0;
	}
	return 1;
}

