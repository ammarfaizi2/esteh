
#include <string.h>
#include <esteh/vm/estehvm.h>
#include <esteh/vm/parser/token.h>

extern char *fmap;
extern size_t fmap_size;

tea_token **tokens;
uint32_t tokens_ptr = 0;

void vm_token_clean_up() {
	for (int i = 0; i < tokens_ptr; ++i) {
		if (tokens[i] != NULL) {
			if (tokens[i]->token != NULL) {
				free(tokens[i]->token);
			}
			free(tokens[i]);
		}
	}
	free(tokens);
}

void vm_lexical() {

	uint32_t lineno = 1;
	size_t used_fmap_size = fmap_size - 3;

	// Init tokens memory allocation.
	size_t allocated_tokens_size = sizeof(tea_token) * 30;
	tokens = (tea_token **)malloc(allocated_tokens_size);

	#define TOKENS_REALLOC \
		if (allocated_tokens_size <= (tokens_ptr * (sizeof(tea_token)))) { \
			allocated_tokens_size += sizeof(tea_token) * 30; \
			tokens = (tea_token **)realloc(); \
		}

	for (size_t i = 0; i < used_fmap_size; ++i) {

		// Comment parsers.
		if (fmap[i] == '/') {
			// Multi lines comment.
			if (fmap[i + 1] == '*') {
				i += 2;
				while (!((fmap[i] == '*') && (fmap[i + 1] == '/'))) {
					i++;
					if (fmap[i] == '\n') lineno++;
				}
			}

			// Singeline comment.
			if (fmap[i + 1] == '/') {
				i += 2;
				while (fmap[i] != '\n')	i++;
				lineno++;
			}
		}

		// ut_constant parser
		if (
			((fmap[i] >= 'a') && (fmap[i] <= 'z')) ||
			((fmap[i] >= 'A') && (fmap[i] <= 'Z')) ||
			((fmap[i] >= 128)) ||
			((fmap[i] == '_'))
		) {

			uint32_t token_ptr  = 0;
			uint32_t allocated_token_size = sizeof(char) * 10;

			char *token = (char *)malloc(allocated_token_size);
			token[token_ptr] = fmap[i];
			token_ptr++;
			i++;

			while(
				((fmap[i] >= 'a') && (fmap[i] <= 'z')) ||
				((fmap[i] >= 'A') && (fmap[i] <= 'Z')) ||
				((fmap[i] >= '0') && (fmap[i] <= '9')) ||
				((fmap[i] >= 128)) ||
				((fmap[i] == '_'))
			) {
				if (allocated_token_size <= token_ptr) {
					allocated_token_size += 10;
					token = (char *)realloc(token, allocated_token_size);
				}

				token[token_ptr] = fmap[i];
				token_ptr++;
				i++;
			}

			tokens[tokens_ptr] = (tea_token *)malloc(sizeof(tea_token));
			tokens[tokens_ptr]->token_type = ut_constant;
			tokens[tokens_ptr]->lineno = lineno;
			tokens[tokens_ptr]->token = (char *)malloc(token_ptr);
			memcpy(tokens[tokens_ptr]->token, token, token_ptr);

			tokens_ptr++;
			free(token);
			token = NULL;
		}
	}

}
