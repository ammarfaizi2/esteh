
#ifndef HAKO_FILE_STREAMER_HPP
#define HAKO_FILE_STREAMER_HPP

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
