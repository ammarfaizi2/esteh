

#ifndef ESTEH__VM__PARSER__TOKEN_H
#define ESTEH__VM__PARSER__TOKEN_H

#include <stdint.h>

enum _token_type {
	ut_whitespace = 0,
	ut_constant = 1,
	ut_variable = 2,
	ut_semicolon = 3
	ut_string = 4,
	ut_number = 5
};

typedef struct _tea_token {
	char *token;
	uint32_t lineno;
	uint32_t token_size;
	enum _token_type token_type;
} tea_token;

#endif
