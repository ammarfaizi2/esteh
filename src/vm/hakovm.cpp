
#include <hako/error.hpp>
#include <hako/hako_opt.hpp>
#include <hako/vm/hakovm.hpp>
#include <hako/vm/code_parser.hpp>

hakovm::hakovm(char *filename, int opt_count, hako_opt **opts) {
	this->filename = filename;
	this->opt_count = opt_count;
	this->opts = opts;
}

void hakovm::run() {
	for (int i = 0; i < this->opt_count; ++i) {
		switch (this->opts[i]->opt_code) {
			case OPT_LINTER_ONLY:
				char *error = nullptr;
				if (this->linter(&error)) {
					printf("No syntax error detected in \"%s\"\n", this->filename);
					exit(0);
				} else {
					hako_error(error);
					exit(1);
				}
				return;
			break;
		}
	}

	this->parse_file(0);
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

int hakovm::linter(char **error) {
	*error = (char*)malloc(sizeof(char));

	this->parse_file(1);

	if (this->error_parse == nullptr) {
		return 1;
	} else {
		return 0;
	}
}

void hakovm::parse_file(int linter_only) {
	this->init_file_streamer();

	code_parser *parser = new code_parser();
	char *buf = (char*)malloc(sizeof(char) * (HAKO_FILE_BUFFER + 1));

	size_t read_bytes;

	while (!feof(this->hdf)) {
		read_bytes = fread(buf, 1, HAKO_FILE_BUFFER, this->hdf);
		parser->buf_read(buf, read_bytes);
		parser->build_opcode();
		if (!parser->is_ok()) {
			memcpy(this->error_parse, parser->get_error(), parser->get_error_length());
			break;
		}
	}

	free(buf); buf = nullptr;

	parser->finish();

	free(parser);
	parser = nullptr;

}
