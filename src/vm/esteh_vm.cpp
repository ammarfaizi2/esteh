
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

				switch ($opx->op1_type) {
					case static_value:						
						esteh_print($opx->op1.static_value);
					break;
					case opcode_1:

						int64_t result = 0;
						uint8_t result_type = $opx->op1.opcode_1->result.static_value.type;

						switch ($opx->op1.opcode_1->code) {
							case TF_ADD:
								result = (
									$opx->op1.opcode_1->op1.static_value.value.lval + 
									$opx->op1.opcode_1->op2.static_value.value.lval
								);
							break;
							case TF_MIN:
								result = (
									$opx->op1.opcode_1->op1.static_value.value.lval - 
									$opx->op1.opcode_1->op2.static_value.value.lval
								);
							break;

							case TF_MUL:
								result = (
									$opx->op1.opcode_1->op1.static_value.value.lval * 
									$opx->op1.opcode_1->op2.static_value.value.lval
								);
							break;

							case TF_DIV:
								result = (
									$opx->op1.opcode_1->op1.static_value.value.lval /
									$opx->op1.opcode_1->op2.static_value.value.lval
								);
							break;
						}

						$opx->op1.static_value.value.lval = result;
						$opx->op1.static_value.type = result_type;
						esteh_print($opx->op1.static_value);
					break;
				}
			break;

			default:
				printf("%d\n", $opx->code);
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