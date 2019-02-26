
#include <stdint.h>
#include <esteh/vm/opcode.hpp>
#include <esteh/vm/code_parser.hpp>

uint16_t code_parser::token_d(char *token, bool *must_be_closed, bool *must_have_operand) {


	#define CP(val) (!strcmp(token, val))

	if (CP("p") || CP("print") || CP("echo")) {
		*must_be_closed = true;
		*must_have_operand = true;
		return TD_PRINT;
	}

	*must_have_operand = false;
	*must_be_closed = false;

	return T_UNKNOWN;
}

const char *get_token_name(uint16_t tkn) {

	#define rq(r,m) case r: return m

	switch (tkn) {
		rq(TD_PRINT, "TD_PRINT");
	}

	return "";
}
