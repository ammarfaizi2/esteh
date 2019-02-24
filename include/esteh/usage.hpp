
#ifndef ESTEH_USAGE_HPP
#define ESTEH_USAGE_HPP

#include <cstdio>
#include <stdlib.h>
#include <string.h>

class usage
{
public:
	usage(char *);
	void general();
private:
	char *appname;
};

#endif
