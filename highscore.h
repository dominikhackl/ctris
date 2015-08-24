#ifndef _HIGHSCORE_H
#define _HIGHSCORE_H

#include "ctris.h"

struct entry_struct
{
	char name[40];
	unsigned int score;
};

struct highscore_struct
{
	struct entry_struct entry[10];
};


void init_highscore(struct highscore_struct *);
char read_highscore(struct highscore_struct *);
void sort_entries(struct highscore_struct *);
char write_highscore(struct highscore_struct *);
char add_to_highscore(const char *, const unsigned int);
char in_highscore(const unsigned int);


#endif

