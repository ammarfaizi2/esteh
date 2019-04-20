
#include <string.h>
#include <stdbool.h>
#include <esteh/vm/estehvm.h>
#include <esteh/vm/parser/token.h>
#include <esteh/vm/debugger/token_dumper.h>

extern char *fmap;
extern size_t fmap_size;

inline static bool comment_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i);
inline static bool ut_string_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i);
inline static bool ut_symbol_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i);
inline static bool ut_number_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i);
inline static bool ut_constant_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i);
inline static bool ut_whitespace_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i);

/**
 * @return Token size.
 */
uint32_t esteh_vm_lexical_analyze(char *fmap, size_t fmap_size, tea_token **tokens) {

	uint32_t lineno = 1;
	uint32_t tokens_ptr = 0;

	size_t allocated_tokens_size = TOKENS_CYCLE_ALLOC;

	for (size_t i = 0; i < fmap_size; ++i) {

		if (comment_parser(tokens, fmap, fmap_size, &tokens_ptr, &lineno, &i)) {
			goto realloc_check;
		}
		
		if (ut_symbol_parser(tokens, fmap, fmap_size, &tokens_ptr, &lineno, &i)) {
			goto realloc_check;
		}

		if (ut_constant_parser(tokens, fmap, fmap_size, &tokens_ptr, &lineno, &i)) {
			goto realloc_check;
		}
		
		if (ut_number_parser(tokens, fmap, fmap_size, &tokens_ptr, &lineno, &i)) {
			goto realloc_check;
		}

		if (ut_string_parser(tokens, fmap, fmap_size, &tokens_ptr, &lineno, &i)) {
			goto realloc_check;
		}

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
				(*tokens)[i]->token = NULL;
			}
			free((*tokens)[i]);
			(*tokens)[i] = NULL;
		}
	}
	free(*tokens);
	*tokens = NULL;
}



inline static bool comment_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i) {
	if (fmap[(*i)] == '/') {
		char token[1];

		// Multi lines comment.
		if (fmap[(*i) + 1] == '*') {
			(*i) += 2;
			while (
				((*i) < fmap_size) && 
				(!((fmap[(*i)] == '*') && (fmap[(*i) + 1] == '/')))
			) {
				(*i)++;
				if (fmap[(*i)] == '\n') (*lineno)++;
			}
		}

		// Singeline comment.
		if (fmap[(*i) + 1] == '/') {
			(*i) += 2;
			while (((*i) < fmap_size) && fmap[(*i)] != '\n')	(*i)++;
			(*lineno)++;
		}

		if (((*tokens_ptr) > 0) && (tokens[(*tokens_ptr) - 1]->token_type == ut_whitespace)) {
			return true;
		}
		token[0] = ' ';
		token[1] = '\0';
		tokens[(*tokens_ptr)] = (tea_token *)malloc(sizeof(tea_token));
		tokens[(*tokens_ptr)]->token_type = ut_whitespace;
		tokens[(*tokens_ptr)]->lineno = (*lineno);
		tokens[(*tokens_ptr)]->token = (char *)malloc(2);
		tokens[(*tokens_ptr)]->token_size = 2;
		memcpy(tokens[(*tokens_ptr)]->token, token, 2);
		(*tokens_ptr)++;
		return true;
	}
	return false;
}

inline static bool ut_string_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i) {
	if (fmap[(*i)] == '"') {
		(*i)++;

		uint32_t token_ptr  = 0;
		uint32_t allocated_token_size = sizeof(char) * 10;

		char *token = (char *)malloc(allocated_token_size);
		token[token_ptr] = fmap[(*i)];
		token_ptr++;
		(*i)++;

		while(((*i) < fmap_size) && (fmap[(*i)] != '"')) {
			if (allocated_token_size <= token_ptr) {
				allocated_token_size += 10;
				token = (char *)realloc(token, allocated_token_size);
			}

			token[token_ptr] = fmap[(*i)];
			token_ptr++;
			(*i)++;
		}

		token[token_ptr] = '\0';
		tokens[(*tokens_ptr)] = (tea_token *)malloc(sizeof(tea_token));
		tokens[(*tokens_ptr)]->token_type = ut_string;
		tokens[(*tokens_ptr)]->lineno = (*lineno);
		tokens[(*tokens_ptr)]->token = (char *)malloc(token_ptr + 1);
		tokens[(*tokens_ptr)]->token_size = token_ptr + 1;
		memcpy(tokens[(*tokens_ptr)]->token, token, token_ptr + 1);
		(*tokens_ptr)++;
		free(token);
		token = NULL;
		return true;
	}
	return false;
}


inline static bool ut_symbol_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i) {
	if (
		(
			// 33 <= c <= 47
			(fmap[(*i)] >= '!' && fmap[(*i)] <= '/') ||

			// 58 <= c <= 64
			(fmap[(*i)] >= ':' && fmap[(*i)] <= '@') ||

			// 91 <= c <= 96
			(fmap[(*i)] >= '[' && fmap[(*i)] <= '`') ||

			// 123 <= c <= 127
			(fmap[(*i)] == '{' && fmap[(*i)] <= '~')
		) && (fmap[(*i)] != '"')
	) {
		uint32_t token_ptr  = 0;
		uint32_t allocated_token_size = sizeof(char) * 10;

		char *token = (char *)malloc(allocated_token_size);
		token[token_ptr] = fmap[(*i)];
		token_ptr++;
		(*i)++;

		while(
			((*i) < fmap_size) && (
				// 33 <= c <= 47
				(fmap[(*i)] >= '!' && fmap[(*i)] <= '/') ||

				// 58 <= c <= 64
				(fmap[(*i)] >= ':' && fmap[(*i)] <= '@') ||

				// 91 <= c <= 96
				(fmap[(*i)] >= '[' && fmap[(*i)] <= '`') ||

				// 123 <= c <= 127
				(fmap[(*i)] == '{' && fmap[(*i)] <= '~')
			) && (fmap[(*i)] != '"')
		) {
			if (allocated_token_size <= token_ptr) {
				allocated_token_size += 10;
				token = (char *)realloc(token, allocated_token_size);
			}

			token[token_ptr] = fmap[(*i)];
			token_ptr++;
			(*i)++;
		}

		token[token_ptr] = '\0';
		tokens[(*tokens_ptr)] = (tea_token *)malloc(sizeof(tea_token));
		tokens[(*tokens_ptr)]->token_type = ut_symbol;
		tokens[(*tokens_ptr)]->lineno = (*lineno);
		tokens[(*tokens_ptr)]->token = (char *)malloc(token_ptr + 1);
		tokens[(*tokens_ptr)]->token_size = token_ptr + 1;
		memcpy(tokens[(*tokens_ptr)]->token, token, token_ptr + 1);

		(*tokens_ptr)++;
		free(token);
		token = NULL;
		return true;
	}
	return false;
}

inline static bool ut_number_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i) {
	if (fmap[(*i)] >= '0' && fmap[(*i)] <= '9') {

		uint32_t token_ptr  = 0;
		uint32_t allocated_token_size = sizeof(char) * 10;

		char *token = (char *)malloc(allocated_token_size);
		token[token_ptr] = fmap[(*i)];
		token_ptr++;
		(*i)++;

		while(((*i) < fmap_size) && ((fmap[(*i)] >= '0') && (fmap[(*i)] <= '9'))) {
			if (allocated_token_size <= token_ptr) {
				allocated_token_size += 10;
				token = (char *)realloc(token, allocated_token_size);
			}

			token[token_ptr] = fmap[(*i)];
			token_ptr++;
			(*i)++;
		}

		token[token_ptr] = '\0';
		tokens[(*tokens_ptr)] = (tea_token *)malloc(sizeof(tea_token));
		tokens[(*tokens_ptr)]->token_type = ut_number;
		tokens[(*tokens_ptr)]->lineno = (*lineno);
		tokens[(*tokens_ptr)]->token = (char *)malloc(token_ptr + 1);
		tokens[(*tokens_ptr)]->token_size = token_ptr + 1;
		memcpy(tokens[(*tokens_ptr)]->token, token, token_ptr + 1);

		(*tokens_ptr)++;
		free(token);
		token = NULL;
		return true;
	}
	return false;
}

inline static bool ut_constant_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i) {
	if (
		((fmap[(*i)] >= 'a') && (fmap[(*i)] <= 'z')) ||
		((fmap[(*i)] >= 'A') && (fmap[(*i)] <= 'Z')) ||
		(fmap[(*i)] >= 128) ||
		(fmap[(*i)] == '_')
	) {

		uint32_t token_ptr  = 0;
		uint32_t allocated_token_size = sizeof(char) * 10;

		char *token = (char *)malloc(allocated_token_size);
		token[token_ptr] = fmap[(*i)];
		token_ptr++;
		(*i)++;

		while(
			((*i) < fmap_size) && (
				((fmap[(*i)] >= 'a') && (fmap[(*i)] <= 'z')) ||
				((fmap[(*i)] >= 'A') && (fmap[(*i)] <= 'Z')) ||
				((fmap[(*i)] >= '0') && (fmap[(*i)] <= '9')) ||
				(fmap[(*i)] >= 128) ||
				(fmap[(*i)] == '_')
			)
		) {
			if (allocated_token_size <= token_ptr) {
				allocated_token_size += 10;
				token = (char *)realloc(token, allocated_token_size);
			}

			token[token_ptr] = fmap[(*i)];
			token_ptr++;
			(*i)++;
		}

		token[token_ptr] = '\0';
		tokens[(*tokens_ptr)] = (tea_token *)malloc(sizeof(tea_token));
		tokens[(*tokens_ptr)]->token_type = ut_constant;
		tokens[(*tokens_ptr)]->lineno = (*lineno);
		tokens[(*tokens_ptr)]->token = (char *)malloc(token_ptr + 1);
		tokens[(*tokens_ptr)]->token_size = token_ptr + 1;
		memcpy(tokens[(*tokens_ptr)]->token, token, token_ptr + 1);

		(*tokens_ptr)++;
		free(token);
		token = NULL;
		return true;
	}
	return false;
}

inline static bool ut_whitespace_parser(tea_token **tokens, char *fmap, size_t fmap_size, uint32_t *tokens_ptr, uint32_t *lineno, size_t *i) {
	if (
		(fmap[(*i)] == '\n') ||
		(fmap[(*i)] == ' ')  ||
		(fmap[(*i)] == '\r') ||
		(fmap[(*i)] == '\t')
	) {
		char token[1];

		if (fmap[(*i)] == '\n') {
			(*lineno)++;
		}

		if (((*tokens_ptr) > 0) && (tokens[(*tokens_ptr) - 1]->token_type == ut_whitespace)) {
			return true;
		}

		token[0] = fmap[(*i)];
		token[0] = '\0';
		tokens[(*tokens_ptr)] = (tea_token *)malloc(sizeof(tea_token));
		tokens[(*tokens_ptr)]->token_type = ut_symbol;
		tokens[(*tokens_ptr)]->lineno = (*lineno);
		tokens[(*tokens_ptr)]->token = (char *)malloc(2);
		tokens[(*tokens_ptr)]->token_size = 2;
		memcpy(tokens[(*tokens_ptr)]->token, token, 2);
		return true;
	}
}