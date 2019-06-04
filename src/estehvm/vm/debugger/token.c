
#include <estehvm/vm/token.h>
#include <estehvm/vm/debugger/token.h>

const char *get_token_type_name(esteh_token_type n) {

	#define _RC(A,B) \
		case A: return B;

	switch (n) {
		_RC(t_whitespace, "t_whitespace")
		_RC(t_string, "t_string")
		_RC(t_number, "t_number")
		_RC(t_symbol, "t_symbol")
		_RC(t_constant, "t_constant")
		_RC(t_operator, "t_operator")
	}

	#undef _RC

	return "unknown";
}
