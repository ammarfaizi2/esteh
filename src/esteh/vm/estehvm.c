
#include <esteh/vm/estehvm.h>

extern *filename;

void estehvm() {
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("Cannot read file: %s\n", filename);
	}
}
