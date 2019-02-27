
#include <esteh/vm/esteh_vm.hpp>
#include <esteh/vm/esteh_token.hpp>
#include <esteh/vm/parsers/esteh_lexical.hpp>

esteh_vm::esteh_vm(char *filename, opt_struct **opts) {
	this->filename = filename;
	this->opts = opts;
}

void esteh_vm::run() {
	this->lexical_analyzer();
}

void init_file() {

}

void esteh_vm::lexical_analyzer() {
	esteh_lexical *st = new esteh_lexical(this->filename, &this->tokens);
	st->run();
	delete st;
}
