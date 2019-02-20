
#ifndef __HAKO_USAGE_H
#define __HAKO_USAGE_H

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
