
#if defined(ESTEH_DEBUG)

#include <esteh/vm/debugger/icetea_debugger.h>

#define SWR(A,B) case A: return B

char *get_token_type(uint8_t tkn_type) {
	switch (tkn_type) {
		SWR(t_unknown, "t_unknown");
		SWR(t_keyword, "t_keyword");
		SWR(t_symbol, "t_symbol");
		SWR(t_constant, "t_constant");
		SWR(t_operator, "t_operator");
		SWR(t_whitespace, "t_whitespace");
		SWR(t_semicolon, "t_semicolon");
	}
	return "undefined_token_type";
}

char *get_token_name(uint16_t tkn_code) {
	switch (tkn_code) {
		SWR(T_NOT_DEFINED_YET, "T_NOT_DEFINED_YET");
		SWR(T_PRINT, "T_PRINT");
		SWR(T_WHITESPACE, "T_WHITESPACE");
		SWR(T_NULL, "T_NULL");
		SWR(T_NUMBER, "T_NUMBER");
		SWR(T_STRING, "T_STRING");
		SWR(T_SEMICOLON, "T_SEMICOLON");
	}
	return "undefined_token_name";
}

#endif
