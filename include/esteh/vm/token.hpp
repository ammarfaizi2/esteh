
#ifndef ESTEH_VM_TOKEN_HPP
#define ESTEH_VM_TOKEN_HPP

#include <stdint.h>
#include <esteh/vm/esteh_token.hpp>

#define TOKEN_DUMPER(TKN) { \
	printf("Token\t: (%d) %s\n", TKN->token, get_token_name(TKN->token)); \
	printf("TType\t: "); \
	printf("%s\n", get_token_type(TKN)); \
	printf("Line \t: %d\n", TKN->line); \
	if (TKN->type == t_constant) { \
		printf("------------------\n"); \
		printf("VType\t: %s\n", get_value_type(TKN->val_type)); \
		switch (TKN->val_type) { \
			case ESTEH_NULL: \
				printf("Value\t: (null)\n"); \
			break; \
			case ESTEH_INT: \
				printf("Value\t: %lld\n", TKN->val.lval); \
			break; \
			case ESTEH_STRING: \
				printf("Value\t: \"%s\"\n", TKN->val.str); \
			break; \
		} \
	} \
	printf("=======================\n"); \
}


void node_dumper(tea_node *node, int init = 1);
const char *get_token_name(uint16_t tkn);
const char *get_value_type(uint16_t tkn);
const char *get_token_type(esteh_token *tknd);

#endif
