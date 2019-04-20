
#include <esteh/vm/parser/token.h>
#include <esteh/vm/buffer/helpers.h>
#include <esteh/vm/debugger/token_dumper.h>

const char *esteh_token_type(enum _token_type type) {
	switch (type) {
		case ut_whitespace:
			return "ut_whitespace";
		break;
		case ut_constant:
			return "ut_constant";
		break;
		case ut_variable:
			return "ut_variable";
		break;
		case ut_symbol:
			return "ut_symbol";
		break;
		case ut_string:
			return "ut_string";
		break;
		case ut_number:
			return "ut_number";
		break;
		default:
			return "Unknown pre-token";
		break;
	}
}

void esteh_token_dumper(tea_token **tokens, uint32_t token_amount) {
	for (int i = 0; i < token_amount; ++i) {
		if (tokens[i] != NULL) {
			esteh_printf("=============================\n");
			esteh_printf("token_type : %s\n", esteh_token_type(tokens[i]->token_type));
			esteh_printf("line_number: %d\n", tokens[i]->lineno);
			if (tokens[i]->token != NULL) {
				esteh_printf("token_body: %s\n", tokens[i]->token);
			}
			esteh_printf("=============================\n");
		}
	}
}
