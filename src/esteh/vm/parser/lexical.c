
#include <string.h>
#include <esteh/vm/estehvm.h>
#include <esteh/vm/parser/token.h>
#include <esteh/vm/debugger/token_dumper.h>

extern char *fmap;
extern size_t fmap_size;

/**
 * @return Token size.
 */
uint32_t esteh_vm_lexical_analyze(char *fmap, size_t fmap_size, tea_token **tokens) {

	uint32_t lineno = 1;
	uint32_t tokens_ptr = 0;

	size_t allocated_tokens_size = TOKENS_CYCLE_ALLOC;

	for (size_t i = 0; i < fmap_size; ++i) {

		char token[1];
		size_t token_ptr = 1;

		if (
			(fmap[i] == '\n') ||
			(fmap[i] == ' ')  ||
			(fmap[i] == '\r') ||
			(fmap[i] == '\t')
		) {
			if (fmap[i] == '\n') {
				lineno++;
			}
			token[0] = fmap[i];
			goto ut_whitespace;
		}

		// Comment parsers.
		if (fmap[i] == '/') {
			// Multi lines comment.
			if (fmap[i + 1] == '*') {
				i += 2;
				while (
					(i < fmap_size) && 
					(!((fmap[i] == '*') && (fmap[i + 1] == '/')))
				) {
					i++;
					if (fmap[i] == '\n') lineno++;
				}
			}

			// Singeline comment.
			if (fmap[i + 1] == '/') {
				i += 2;
				while ((i < fmap_size) && fmap[i] != '\n')	i++;
				lineno++;
			}

			token[0] = ' ';
			continue;
		}

		// ut_constant parser
		if (
			((fmap[i] >= 'a') && (fmap[i] <= 'z')) ||
			((fmap[i] >= 'A') && (fmap[i] <= 'Z')) ||
			(fmap[i] >= 128) ||
			(fmap[i] == '_')
		) {

			uint32_t token_ptr  = 0;
			uint32_t allocated_token_size = sizeof(char) * 10;

			char *token = (char *)malloc(allocated_token_size);
			token[token_ptr] = fmap[i];
			token_ptr++;
			i++;

			while(
				(i < fmap_size) && (
					((fmap[i] >= 'a') && (fmap[i] <= 'z')) ||
					((fmap[i] >= 'A') && (fmap[i] <= 'Z')) ||
					((fmap[i] >= '0') && (fmap[i] <= '9')) ||
					(fmap[i] >= 128) ||
					(fmap[i] == '_')
				)
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
			tokens[tokens_ptr]->token_size = token_ptr;
			memcpy(tokens[tokens_ptr]->token, token, token_ptr);

			tokens_ptr++;
			free(token);
			token = NULL;
		}

		// ut_string parser
		if (fmap[i] == '"') {
			i++;

			uint32_t token_ptr  = 0;
			uint32_t allocated_token_size = sizeof(char) * 10;

			char *token = (char *)malloc(allocated_token_size);
			token[token_ptr] = fmap[i];
			token_ptr++;
			i++;

			while((i < fmap_size) && (fmap[i] != '"')) {
				if (allocated_token_size <= token_ptr) {
					allocated_token_size += 10;
					token = (char *)realloc(token, allocated_token_size);
				}

				token[token_ptr] = fmap[i];
				token_ptr++;
				i++;
			}

			tokens[tokens_ptr] = (tea_token *)malloc(sizeof(tea_token));
			tokens[tokens_ptr]->token_type = ut_string;
			tokens[tokens_ptr]->lineno = lineno;
			tokens[tokens_ptr]->token = (char *)malloc(token_ptr);
			tokens[tokens_ptr]->token_size = token_ptr;
			memcpy(tokens[tokens_ptr]->token, token, token_ptr);
			tokens_ptr++;
			free(token);
			token = NULL;
		}

		goto realloc_check;
	
ut_whitespace:
		tokens[tokens_ptr] = (tea_token *)malloc(sizeof(tea_token));
		tokens[tokens_ptr]->token_type = ut_whitespace;
		tokens[tokens_ptr]->lineno = lineno;
		tokens[tokens_ptr]->token = (char *)malloc(token_ptr);
		tokens[tokens_ptr]->token_size = token_ptr;
		memcpy(tokens[tokens_ptr]->token, token, token_ptr);
		tokens_ptr++;

realloc_check:
		if (allocated_tokens_size <= (tokens_ptr * (sizeof(tea_token)))) { \
			allocated_tokens_size += TOKENS_CYCLE_ALLOC; \
			tokens = (tea_token **)realloc(tokens, allocated_tokens_size); \
		}
	}

	return tokens_ptr;
}

void esteh_vm_token_clean_up(tea_token ***tokens, uint32_t amount) {
	for (int i = 0; i < amount; ++i) {
		if ((*tokens)[i] != NULL) {
			if ((*tokens)[i]->token != NULL) {
				free((*tokens)[i]->token);
			}
			free((*tokens)[i]);
		}
	}
	free(*tokens);
}
