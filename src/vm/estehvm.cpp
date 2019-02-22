
#include <sys/wait.h>
#include <sys/types.h>
#include <esteh/error.hpp>
#include <esteh/esteh_opt.hpp>
#include <esteh/vm/estehvm.hpp>
#include <esteh/vm/code_parser.hpp>

estehvm::estehvm(char *filename, int opt_count, esteh_opt **opts) {
	this->filename = filename;
	this->opt_count = opt_count;
	this->opts = opts;
}

void estehvm::run() {
	for (int i = 0; i < this->opt_count; ++i) {
		switch (this->opts[i]->opt_code) {
			case OPT_LINTER_ONLY:
				char *error = nullptr;
				if (this->linter(&error)) {
					printf("No syntax error detected in \"%s\"\n", this->filename);
					exit(0);
				} else {
					esteh_error(error);
					exit(1);
				}
				return;
			break;
		}
	}

	this->parse_file(0);
}

void estehvm::init_file_streamer() {

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
		esteh_error("Could not open input file: \"%s\"", this->filename);
}

int estehvm::linter(char **error) {
	*error = (char*)malloc(sizeof(char));

	this->parse_file(1);

	if (this->error_parse == nullptr) {
		return 1;
	} else {
		return 0;
	}
}

void estehvm::parse_file(int linter_only) {
	this->init_file_streamer();
	int pid;

	if (!(pid = fork())) {
		code_parser *parser = new code_parser();
		parser->add_file_handler(this->hdf, this->error_parse);
		parser->build_opcode();
		parser->finish();
		free(parser);
		parser = nullptr;
		sleep(10);
		exit(0);
	}

	int status;
	waitpid(pid, &status, WUNTRACED);

	printf("End\n");
	sleep(100);
}
