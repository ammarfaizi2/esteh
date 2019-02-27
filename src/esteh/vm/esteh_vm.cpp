
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <esteh/vm/esteh_vm.hpp>
#include <esteh/vm/esteh_token.hpp>
#include <esteh/vm/parsers/esteh_lexical.hpp>

esteh_vm::esteh_vm(char *filename, opt_struct **opts) {
	this->filename = filename;
	this->opts = opts;
}

esteh_vm::run() {
	this->open_file();
	this->lexical_analyzer();
}

void init_file() {

}

void esteh_vm::lexical_analyzer() {
	this->tokens = (esteh_token **)malloc(ESTEH_TOKEN_FIRST_ALLOC);
	esteh_lexical *st = new esteh_lexical(this->filename, &this->tokens);
	st->run();
	delete st;
}
