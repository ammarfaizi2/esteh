
#include <hako/error.hpp>
#include <hako/hakovm.hpp>
#include <hako/hako_opt.hpp>

#define HAKO_FILE_BUFFER 2048

hakovm::hakovm(char *filename, int opt_count, hako_opt **opts) {
	this->filename = filename;
	this->opt_count = opt_count;
	this->opts = opts;
}

void hakovm::run() {

	this->init_file_streamer();
	this->parse_file();

}

void hakovm::init_file_streamer() {

	// Make sure that the file exists and readable.
	if(!(access(this->filename, F_OK | R_OK) != -1)) {
		goto init_file_error;
	}

	// Open a file handle to read.
	this->hdf = fopen(this->filename, "r");

	if (this->hdf == NULL) {
		goto init_file_error;
	}

	return;

	init_file_error:
		hako_error("Could not open input file: \"%s\"", this->filename);
}

void hakovm::parse_file() {
	char *buf = (char*)malloc(sizeof(char) * (HAKO_FILE_BUFFER + 1));

	while (!feof(this->hdf)) {
		fread(buf, 1, HAKO_FILE_BUFFER, this->hdf);
		printf("%s\n", buf);
	}
}
