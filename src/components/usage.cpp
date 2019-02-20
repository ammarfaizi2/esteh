
#include <hako/usage.hpp>

usage::usage(char *appname) {
	size_t l = strlen(appname) + 1;
	this->appname = (char*)malloc(sizeof(char) * l);
	memcpy(this->appname, appname, l);
}

void usage::general() {
	printf("Usage: %s [options] <file>\n", this->appname);
}
