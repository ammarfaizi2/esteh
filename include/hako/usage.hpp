
#ifndef HAKO_USAGE_H
#define HAKO_USAGE_H

#include <cstdio>
#include <stdlib.h>
#include <string.h>

class usage
{
public:
	usage(char *);
	~usage();
	void general();
private:
	char *appname;
};

#endif
