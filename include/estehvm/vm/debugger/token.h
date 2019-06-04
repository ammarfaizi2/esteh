
#ifndef estehvm__vm__debugger__token_h
#define estehvm__vm__debugger__token_h

#include <stdio.h>
#include <estehvm/vm/stdio.h>
#include <estehvm/vm/token.h>

#define TOKEN_DUMPER(TOKENS,TOKEN_COUNT) \
	do { \
		for (int __i = 0; __i < TOKEN_COUNT; ++__i) { \
			esteh_printf("===============================\n"); \
			esteh_printf("Token Type : %s\n", get_token_type_name(TOKENS[__i]->type)); \
			esteh_printf("Body Size  : %ld\n", TOKENS[__i]->body_size); \
			esteh_printf("Token Body : %s\n", TOKENS[__i]->body); \
			esteh_printf("===============================\n"); \
		} \
	} while (0)

const char *get_token_type_name(esteh_token_type);

#endif
