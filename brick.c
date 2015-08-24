#include "ctris.h"
#include "brick.h"

char brick_digit[7][4][4] =
{
	{
		{0, 1, 0, 0},
		{0, 2, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 0},
	},
	{
		{2, 2, 0, 0},
		{2, 2, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	},
	{
		{1, 1, 0, 0},
		{0, 2, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0},
	},
	{
		{0, 1, 1, 0},
		{0, 2, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0},
	},
	{
		{1, 1, 0, 0},
		{0, 2, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	},
	{	
		{0, 1, 1, 0},
		{1, 2, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	},
	{
		{0, 1, 0, 0},
		{1, 2, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	}
};

void change_direction(char board[BOARD_HEIGHT][BOARD_WIDTH], char cur_brick[4][4], const unsigned char x, const unsigned char y, const char i)
{
	char new_brick[4][4];
	unsigned char n, s;
	
	for(n = 0; n < 4; n++)
	{
		for(s = 0; s < 4; s++)
		{
			if(i < 0)
			{
				new_brick[n][s] = cur_brick[3 - s][n];
			}
			else
			{
				new_brick[n][s] = cur_brick[s][3 - n];
				
			}
		}
	}
	
	if(check_brick(board, new_brick, x, y) == 0)
	{
		memcpy(cur_brick, new_brick, sizeof(char) * 4 * 4);
	}
}

void find_index(char cur_brick[4][4], unsigned char *x, unsigned char *y)
{
	unsigned char i, n;
	for(i = 0; i < 4; i++)
	{
		for(n = 0; n < 4 && cur_brick[i][n] != 2; n++);
		if(n < 4)
		{
			*x = n;
			*y = i;
			return;
		}
	}
	
}

char check_brick(char board[BOARD_HEIGHT][BOARD_WIDTH], char cur_brick[4][4], const unsigned char x, const unsigned char y)
{
	unsigned char a, b, t, i, n;
	if(x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
	{
		return 1;
	}
	find_index(cur_brick, &a, &b);
	t = a;
	for(n = 0; n < 4; n++)
	{
		for(i = 0; i < a && cur_brick[n][i] == 0; i++);
		if(i < t)
		{
			t = i;
		}
	}
	if(x - (a - t) < 0)
	{
		return 1;
	}
	t = a;
	for(n = 0; n < 4; n++)
	{
		for(i = 3; i > a && cur_brick[n][i] == 0; i--);
		if(i > t)
		{
			t = i;
		}
	}
	if(x + (t - a) > BOARD_WIDTH - 1)
	{
		return 1;
	}
	t = b;
	for(n = 0; n < 4; n++)
	{
		for(i = 3; i > b && cur_brick[i][n] == 0; i--);
		if(i > t)
		{
			t = i;
		}
	}
	if(y + (t - b) > BOARD_HEIGHT - 1)
	{
		return 1;
	}

	for(i = 0; i < 4; i++)
	{
		for(n = 0; n < 4; n++)
		{
			if((cur_brick[i][n] == 1 || cur_brick[i][n] == 2) && board[y - b + i][x - a + n] != 0)
			{
				return 1;
			}
		}
	}
	return 0;
}

void draw_to_board(char board[BOARD_HEIGHT][BOARD_WIDTH], char cur_brick[4][4], const char brick_type, const unsigned char x, const unsigned char y)
{
	unsigned char a, b, i, n;
	if(check_brick(board, cur_brick, x, y) != 0)
	{
		return;
	}
	find_index(cur_brick, &a, &b);
	for(i = 0; i < 4; i++)
	{
		for(n = 0; n < 4; n++)
		{
			if(cur_brick[i][n] == 1 || cur_brick[i][n] == 2)
			{
				board[y - b + i][x - a + n] = brick_type;
			}
		}
	}
}

