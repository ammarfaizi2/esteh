
#ifndef __HAKO_FILE_STREAMER_H
#define __HAKO_FILE_STREAMER_H

#include <cstdio>
#include <stdlib.h>

class file_streamer
{
public:
	file_streamer(char *);
	~file_streamer();

	void *read();

private:
	FILE *file;
};

#endif
