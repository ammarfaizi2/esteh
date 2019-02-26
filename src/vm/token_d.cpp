
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

		rq(TE_INT, "TF_INT");
		rq(TE_STRING, "TF_STRING");

		rq(TF_ADD, "TF_ADD");
		rq(TF_MIN, "TF_MIN");
		rq(TF_MUL, "TF_MUL");
		rq(TF_DIV, "TF_DIV");
	}

	return "";
}
