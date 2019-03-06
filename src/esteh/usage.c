
#include <stdio.h>
#include <esteh/usage.h>

void esteh_usage(char *appname) {
	fprintf(stdout, "Usage: %s [option] <file>\n", appname);
}
