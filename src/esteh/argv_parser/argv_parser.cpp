
#include <esteh/argv_parser/argv_parser.hpp>

argv_parser::argv_parser(int argc, char **argv) {
	this->argc = argc;
	this->argv = argv;	
}

void argv_parser::run(opt_struct ***opts) {
	this->parse();
}

void argv_parser::parse() {

	#define $argv this->argv[i]

	for (int i = 0; i < this->argc; ++i) {
		
	}
}

bool argv_parser::is_error() {
	return this->error != nullptr;
}

char *argv_parser::get_error() {
	return this->error;
}
