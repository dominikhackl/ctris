#include "ctris.h"
#include "screen.h"
#include "game.h"

char game_state = RUNNING_STATE, old_style_keys = 0, counterclockwise_rotation = 0, highscore_file_path[256], default_name[40];
unsigned long rseed = 1;

#ifndef HIDE_CURSOR_WORKAROUND
char hide_cursor_workaround = 0;
#else
char hide_cursor_workaround = 1;
#endif

void handle_quit_signal(const int code)
{
	if(game_state != RUNNING_STATE && game_state != QUIT_STATE)
	{
		restore_screen();
		exit(0);
	}
	game_state = SIGNALED_QUIT_STATE;
}

void handle_resize_signal(const int code)
{
        pause_game();
}

unsigned char get_rand(const unsigned char i)
{
	rseed = rseed * 1103515245 + 12345;
	return (unsigned char)((rseed / 65536) % 32768) % i;
}

int main(int argc, char *argv[])
{
	int optchr;
	char *name;
	
	rseed = time(NULL);
	
	snprintf(highscore_file_path, 256, "%s/.ctris_highscore.dat", getenv("HOME"));
	
	if((name = getenv("USER")) != NULL)
	{
		strncpy(default_name, getenv("USER"), 40);
	}
	else
	{
		default_name[0] = '\0';
	}
	

	signal(SIGKILL, handle_quit_signal);
	signal(SIGWINCH, handle_resize_signal);
	signal(SIGTERM, handle_quit_signal);
	signal(SIGINT, handle_quit_signal);
	signal(SIGQUIT, handle_quit_signal);
	while((optchr = getopt(argc, argv, "hvroisc")) != EOF)
	{
		switch(optchr)
		{
			case 'h':
				printf("Help:\n\
-h             parameter list\n\
-o             use old style keys\n\
-v             version info\n\
-i             enable hide cursor workaround\n\
-r             remove highscore-file\n\
-c             enable couterclockwise rotation\n");
				game_state = QUIT_STATE;
				break;
			case 'v':
				printf("\
ctris %s, built on %s.\n\
(c) Dominik Hackl [dominik@hackl.dhs.org]\n\
Homepage: http://www.hackl.dhs.org/ctris\n\
ALL PARTS OF CTRIS ARE UNDER THE TERMS OF GPL. SEE DOCUMENTATION FOR MORE INFO\n\
", VERSION, __DATE__);
				game_state = QUIT_STATE;
				break;
			case 'r':
				printf("removing highscore-file (\"%s\")...", highscore_file_path);
				if(remove(highscore_file_path) == 0)
				{
					printf("done\n");
				}
				else
				{
					printf("failed\n");
				}
				game_state = QUIT_STATE;
				break;
			case 'o':
				old_style_keys = 1;
				break;
			case 'i':
				hide_cursor_workaround = 1;
			case 'c':
				counterclockwise_rotation = 1;
			default:
				break;
		}
	}
	if(game_state != RUNNING_STATE)
	{
		return 0;
	}
	if(init_screen() != 0)
	{
		return 1;
	}
	do
	{
		start_game();
	} while(game_state == QUIT_STATE && play_again() ==  0);
	restore_screen();
	return 0;
}


