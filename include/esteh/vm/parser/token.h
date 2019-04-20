
#ifndef ESTEH__VM__PARSER__TOKEN_H
#define ESTEH__VM__PARSER__TOKEN_H

#include <stdlib.h>
#include <stdint.h>

enum _token_type {
	ut_whitespace = 0,
	ut_constant = 1,
	ut_variable = 2,
	ut_symbol = 3,
	ut_string = 4,
	ut_number = 5
};

typedef struct _tea_token {
	char *token;
	uint32_t lineno;
	uint32_t token_size;
	enum _token_type token_type;
} tea_token;

void esteh_vm_token_clean_up(tea_token ***tokens, uint32_t amount);
uint32_t esteh_vm_lexical_analyze(char *fmap, size_t fmap_size, tea_token **tokens);

#define TOKENS_CYCLE_ALLOC (sizeof(tea_token) * 30)

#endif
