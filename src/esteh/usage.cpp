
#include <cstdio>
#include <esteh/usage.hpp>

void esteh_usage(char *appname) {
	printf("Usage: %s [options] <file>\n", appname);
	printf(" Options:\n");
	printf("   -v, --version\tShow EsTeh version number\n");
	printf("   -l\t\t\tSyntax check only (lint)\n");
}
