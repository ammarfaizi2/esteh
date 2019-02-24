

#include <esteh/error.hpp>
#include <esteh/esteh_opt.hpp>
#include <esteh/vm/estehvm.hpp>
#include <esteh/vm/code_parser.hpp>

estehvm::estehvm(char *filename, int opt_count, esteh_opt **opts) {
	this->filename = filename;
	this->opt_count = opt_count;
	this->opts = opts;
}

estehvm::~estehvm() {
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
	code_parser *parser = new code_parser();
	parser->set_file(this->filename);
	parser->build_opcode();
	parser->finish();
	free(parser);
	parser = nullptr;
}