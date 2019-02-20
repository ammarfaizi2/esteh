
#include <hako/file_streamer.hpp>

file_streamer::file_streamer(char *filename) {
	this->file = fopen(filename, "r");
}

void *file_streamer::read() {
	void *buf = malloc(sizeof(char) * 1024);
	fread(buf, 1, 1024, this->file);
	return buf;
}
