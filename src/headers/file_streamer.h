
#ifndef __FILE_STREAMER_H
#define __FILE_STREAMER_H 1

#include <cstdio>
#include <stdlib.h>

class file_streamer
{
public:
	file_streamer(char *);
	~file_streamer();

	char *read();

private:
	FILE *file;
};

#endif