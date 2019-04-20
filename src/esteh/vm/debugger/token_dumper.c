
#include <esteh/vm/parser/token.h>
#include <esteh/vm/buffer/helpers.h>
#include <esteh/vm/debugger/token_dumper.h>

void esteh_token_dumper(tea_token **tokens, uint32_t token_amount) {
	for (int i = 0; i < token_amount; ++i) {
		if (tokens[i] != NULL) {
			esteh_printf("Token Type: %s\n", esteh_token_type(tokens[i]->token_type));
			if (tokens[i]->token != NULL) {
				
			}
		}
	}
}

const char *esteh_token_type(enum _token_type type) {
	switch (type) {
		case ut_whitespace:
			return "ut_whitespace";
		break;
	}
}
