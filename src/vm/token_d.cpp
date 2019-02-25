
#include <stdint.h>
#include <esteh/vm/opcode.hpp>
#include <esteh/vm/code_parser.hpp>

uint16_t code_parser::token_d(char *token) {


	#define CP(val) (!strcmp(token, val))

	if (CP("p") || CP("print") || CP("echo")) {
		return TD_PRINT;
	}
	return T_UNKNOWN;
}
