
#include <stdint.h>
#include <stdlib.h>
#include <esteh/error.hpp>
#include <esteh/esteh_opt.hpp>
#include <esteh/vm/esteh_vm.hpp>
#include <esteh/vm/code_parser.hpp>

#include "executors/esteh_print.hpp"

esteh_vm::esteh_vm(char *filename, int opt_count, esteh_opt **opts) {
	this->filename = filename;
	this->opt_count = opt_count;
	this->opts = opts;
}

void esteh_vm::run() {
	for (int i = 0; i < this->opt_count; ++i) {
		switch (this->opts[i]->opt_code) {
			case OPT_LINTER_ONLY:
				if (this->linter()) {
					printf("No syntax error detected in \"%s\"\n", this->filename);
					exit(0);
				}
				return;
			break;
		}
	}
	this->parse_file();
	this->execute_opcodes();
}

bool esteh_vm::linter() {
	this->parse_file();
	return true;
}

void esteh_vm::parse_file() {
	code_parser *parser = new code_parser();
	parser->set_file(this->filename);
	this->opcode_count = parser->parse_file(&this->opcodes);

	if (!parser->is_ok()) {

		free(this->opcodes);
		this->opcodes = nullptr;

		esteh_error(parser->get_error());

		delete parser;
		parser = nullptr;

		exit(1);
	}

	delete parser;
	parser = nullptr;
}

void esteh_vm::execute_opcodes() {

	#define $opx this->opcodes[i]

	bool skip = false;

	// Run opcode.
	for (uint32_t i = 0; i < this->opcode_count; ++i) {

		if (skip) {
			skip = false;
			continue;
		}

		switch ($opx->code) {
			case TD_PRINT:
				esteh_print($opx->op1);
			break;

			default:
				esteh_error("Unknown opcode in \"%s\" on line \"%d\"", this->filename, $opx->lineno);
				exit(1);
			break;
		}

		free($opx);
		$opx = nullptr;
	}

	free(this->opcodes);
	this->opcodes = nullptr;
}