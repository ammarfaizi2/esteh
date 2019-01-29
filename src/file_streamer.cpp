
#include "headers/file_streamer.h"

file_streamer::file_streamer(char *filename) {
	this->file = fopen(filename, "r");
	free(filename);
}
