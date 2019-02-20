
#ifndef HAKO_USAGE_HPP
#define HAKO_USAGE_HPP

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
