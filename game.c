#include "ctris.h"
#include "game.h"
#include "brick.h"
#include "screen.h"
#include "highscore.h"

void init_board(char board[BOARD_HEIGHT][BOARD_WIDTH])
{
	unsigned char i, n;
	for(i = 0; i < BOARD_HEIGHT; i++)
	{
		for(n = 0; n < BOARD_WIDTH; n++)
		{
			board[i][n] = 0;
		}
	}
}

void remove_this_row(WINDOW *win, char board[BOARD_HEIGHT][BOARD_WIDTH], unsigned char y)
{
	unsigned char x;
	show_remove_row(win, board, y);
	for(; y > 0; y--)
	{
		for(x = 0; x < BOARD_WIDTH; x++)
		{
			board[y][x] = board[y - 1][x];
		}
	}
}

void remove_rows(WINDOW *win, char board[BOARD_HEIGHT][BOARD_WIDTH], unsigned int *score, const char level)
{
	char removed_rows = 0;
	unsigned char x, y;
	unsigned int sub_score = 0;
	for(y = 0; y < BOARD_HEIGHT; y++)
	{
		for(x = 0; x < BOARD_WIDTH; x++)
		{
			if(board[y][x] == 0)
			{
				break;
			}
		}
		if(x >= BOARD_WIDTH)
		{
			remove_this_row(win, board, y);
			removed_rows++;
			sub_score += BONUS_CONST * level * removed_rows;
		}
	}
	*score += sub_score;
	if(removed_rows > 0)
	{
		refresh_win(win);
		usleep(REMOVE_SPLASH_TIME);
	}
}

void calc_level(const unsigned int score, char *level)
{
	while(SPEED_CONST_2 - (*level + 1) * SPEED_CONST_1 >= 0 && score / (LEVEL_CONST * (unsigned int)pow((double)*level, 2)) > 0)
	{
		*level += 1;
	}
}

void pause_game()
{
        game_engine(true);
}

unsigned int start_game()
{
        return game_engine(false);
}

unsigned int game_engine(bool resize)
{
	static char brick_type, next_brick_type, name[40], cur_brick[4][4], board[BOARD_HEIGHT][BOARD_WIDTH], level = 1;
	char run;
	unsigned int score = 0;
    static unsigned long time = 0L;
    unsigned int tick = 0;
	static unsigned char x, y;
	static WINDOW *board_win, *preview_win, *score_win;
    static bool engine_stop = false;

	show_headline();
        
    if (resize) {
        if (game_state != PAUSED_STATE) {
            // if the game engine is in the RUNING_STATE then
            // the resize call below will switch the engine to PAUSED_STATE
            
            // if the game is in the QUIT_STATE then
            // the call below will simulate a key being pressed
            // and that will force the highscore screen or play_again dialog to be refreshed
            put_key('p');
        }

    	if (game_state == QUIT_STATE)
        {
            // if the game is in the highscore screen then refresh it upon resize
            if (!engine_stop) {
                show_highscore(name);
            }
        }
        else
        {
            // here we will only have RUNNING_STATE, PAUSED_STATE or GAME_OVER_STATE
            // both states coresponds to the main screen so
            // the main screen will be refreshed upon resize
            show_score(score_win, score, level, time);
            show_brick_preview(preview_win, next_brick_type);
            show_board_win(board_win, board, cur_brick, brick_type, x, y);
            if (game_state == PAUSED_STATE) {
                    show_pause(board_win);
            }
            else if (game_state == GAME_OVER_STATE) {
                    show_game_over(board_win);
            }
        }
        return score;
    }
    
    engine_stop = false;
	board_win = (WINDOW *)create_board_win();
	preview_win = (WINDOW *)create_preview_win();
	score_win = (WINDOW *)create_score_win();
	init_board(board);
	show_score(score_win, score, level, time);
	next_brick_type = get_rand(7) + 1;
	game_state = PAUSED_STATE;
	wait_for_start(board_win);
	game_state = RUNNING_STATE;
	while(game_state == RUNNING_STATE)
	{
		brick_type = next_brick_type;
		next_brick_type = get_rand(7) + 1;
		show_brick_preview(preview_win, next_brick_type);
		memcpy(cur_brick, brick_digit[brick_type - 1], sizeof(char) * 4 * 4);
		x = BOARD_WIDTH / 2;
		y = 0;
		run = 1;
		while(game_state == RUNNING_STATE)
		{
			show_board_win(board_win, board, cur_brick, brick_type, x, y);
			switch(get_key(board_win))
			{
				case 's':
				case KEY_DOWN:
					if(old_style_keys != 0)
					{
						if(counterclockwise_rotation == 1)
						{
							change_direction(board, cur_brick, x, y, -1);
						}
						else
						{
							change_direction(board, cur_brick, x, y, 1);
						}
					}
					else
					{
						if(check_brick(board, cur_brick, x, y + 1) == 0)
						{
							y++;
						}
					}
					break;
				case 'w':
				case 'k':
				case KEY_UP:
					if(counterclockwise_rotation == 1)
					{
						change_direction(board, cur_brick, x, y, 1);
					}
					else
					{
						change_direction(board, cur_brick, x, y, -1);
					}
					break;
				case 'd':
				case 'l':
				case KEY_RIGHT:
					if(check_brick(board, cur_brick, x + 1, y) == 0)
					{
						x++;
					}
					break;
				case 'a':
				case 'j':
				case KEY_LEFT:
					if(x > 0 && check_brick(board, cur_brick, x - 1, y) == 0)
					{
						x--;
					}
					break;
				case ' ':
					if(old_style_keys != 0)
					{
						if(check_brick(board, cur_brick, x, y + 1) == 0)
						{
							y++;
						}
					}
					else
					{
						while(check_brick(board, cur_brick, x, y + 1) == 0)
						{
							y++;
						}
					}
					break;
				case 'p':
					game_state = PAUSED_STATE;
					show_pause(board_win);
                    while(old_get_key(board_win) != 'p');
					game_state = RUNNING_STATE;
					break;
				case 'q':
					game_state = QUIT_STATE;
					break;
			}
            tick = SPEED_CONST_2 - level * SPEED_CONST_1;
            time += tick;
			show_score(score_win, score, level, time);
			usleep(tick);
			if(run > 15)
			{
				if(check_brick(board, cur_brick, x, y + 1) == 0)
				{
					y++;
					run = 0;
				}
				else
				{
					if(y <= 1)
					{
						game_state = GAME_OVER_STATE;
						show_game_over(board_win);
                        while(old_get_key(board_win) != ' ');
						game_state = QUIT_STATE;
					}
					draw_to_board(board, cur_brick, brick_type, x, y);
					show_board_win(board_win, board, cur_brick, brick_type, x, y);
					remove_rows(board_win, board, &score, level);
#ifdef EXTRA_BONUS
					score += level;
#endif
					calc_level(score, &level);
					show_score(score_win, score, level, time);
					break;
				}
			}
			run++;
		}
		
	}
	destroy_score_win(score_win);
	destroy_preview_win(preview_win);
	destroy_board_win(board_win);
	if(game_state == QUIT_STATE)
	{
		if(in_highscore(score) == 0)
		{
			add_user_to_highscore(name, score);
		}
		else
		{
			strncpy(name, "-no-name-", 40);
		}
		show_highscore(name);
    	getch();
	}
    engine_stop = true;
	return score;
}


