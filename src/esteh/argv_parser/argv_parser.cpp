
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <esteh/macros.hpp>
#include <esteh/version.hpp>
#include <esteh/argv_parser/argv_parser.hpp>

argv_parser::argv_parser(int argc, char **argv, opt_struct ***opts) {
	this->argc = argc;
	this->argv = argv;
	this->opts = opts;
}

void argv_parser::run(char **filename) {

	#define $argv this->argv[i]

	bool got_filename = false;

	for (int i = 0; i < this->argc; ++i) {
		if (this->skip) {
			this->skip = false;
			continue;
		}
		size_t len = strlen($argv);
		if (len == 0) continue;
		if ($argv[0] == '-') {
			if (len == 1) {
				this->error = strdup("Invalid option \"-\"");
				return;
			}
			if (len == 2 && $argv[1] == '-'){
				this->error = strdup("Invalid option \"--\"");
				return;	
			}

			if ($argv[1] != '-') {
				if (this->opt1($argv+1)) {
					return;
				}
			} else if (this->opt2($argv+2)) {
				return;
			}
		}

		if (!got_filename) {
			(*filename) = (char *)malloc(len + 1);
			memcpy(*filename, $argv, len);
			(*filename)[len] = '\0';
			got_filename = true;
		}
	}

}

bool argv_parser::opt1(char *opt) {
	
	switch (*opt) {
		case 'v':
			printf("%s\n", ESTEH_VERSION);
			exit(0);
		break;
	}


	// Return false means OK.
	return false;
}

bool argv_parser::opt2(char *opt) {

	if (SCMP("version", opt)) {
		printf("%s\n", ESTEH_VERSION);
		exit(0);
	}

	// Return false means OK.
	return false;
}

bool argv_parser::is_error() {
	return this->error != nullptr;
}

char *argv_parser::get_error() {
	return this->error;
}
