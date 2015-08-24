#include "ctris.h"
#include "highscore.h"

void init_highscore(struct highscore_struct *highscore)
{
	unsigned char i;
	for(i = 0; i < 10; i++)
	{
		highscore->entry[i].score = 0;
		highscore->entry[i].name[0] = '\0';
	}
}

char read_highscore(struct highscore_struct *highscore)
{
	FILE *highscore_file;
	if((highscore_file = fopen(highscore_file_path, "r")) == NULL)
	{
		init_highscore(highscore);
		return 1;
	}
	fread(highscore, sizeof(struct highscore_struct), 1, highscore_file);
	fclose(highscore_file);
	return 0;
}

void sort_entries(struct highscore_struct *highscore)
{
	struct highscore_struct sorted_highscore;
	unsigned char i, n, highest = 0;
	unsigned int max;
	for(i = 0; i < 10; i++)
	{
		max = 0;
		for(n = 0; n < 10; n++)
		{
			if(highscore->entry[n].score > max)
			{
				max = highscore->entry[n].score;
				highest = n;
			}
		}
		memcpy(&sorted_highscore.entry[i], &highscore->entry[highest], sizeof(struct entry_struct));
		highscore->entry[highest].score = 0;
		highscore->entry[highest].name[0] = '\0';
	}
	memcpy(highscore, &sorted_highscore, sizeof(struct highscore_struct));
}

char write_highscore(struct highscore_struct *highscore)
{
	FILE *highscore_file;
	sort_entries(highscore);
	if((highscore_file = fopen(highscore_file_path, "w")) == NULL)
	{
		return 1;
	}
	fwrite(highscore, sizeof(struct highscore_struct), 1, highscore_file);
	fclose(highscore_file);
	return 0;
}

char add_to_highscore(const char *name, const unsigned int score)
{
	struct highscore_struct highscore;
	if(in_highscore(score) != 0)
	{
		return 1;
	}
	read_highscore(&highscore);
	highscore.entry[9].score = score;
	strncpy(highscore.entry[9].name, name, 40);
	write_highscore(&highscore);
	return 0;
}

char in_highscore(const unsigned int score)
{
	struct highscore_struct highscore;
	read_highscore(&highscore);
	if(score > highscore.entry[9].score)
	{
		return 0;
	}
	return 1;
}

