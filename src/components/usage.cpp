
#include <esteh/usage.hpp>

usage::usage(char *appname) {
	size_t l = strlen(appname) + 1;
	this->appname = (char*)malloc(sizeof(char) * l);
	memcpy(this->appname, appname, l);
}

void usage::general() {
	printf("Usage: %s [options] <file>\n", this->appname);
	printf("\n");
	printf(" Options:\n");
	printf("\t-v, --version\t\t\tVersion number\n");
	printf("\t-l\t\t\tSyntax check only (lint)\n");
}
