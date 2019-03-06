
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <esteh/macros.h>
#include <esteh/vm/esteh_vm.h>
#include <esteh/vm/esteh_token.h>
#include <esteh/vm/parsers/parse_error.h>
#include <esteh/vm/debugger/icetea_debugger.h>

extern int fd;
extern char *fmap;
extern char *filename; // from main.c
extern size_t fmap_size;
extern esteh_token **tokens;
extern uint32_t token_count;
extern size_t token_cur_size;

#define ESTEH_TOKEN_REALLOC \
	if (((token_count + 3) * sizeof(esteh_token *)) >= token_cur_size) { \
		token_cur_size += ESTEH_TOKEN_FIRST_ALLOC; \
		tokens = (esteh_token **)realloc(tokens, token_cur_size); \
	} \

void esteh_token_clean_up() {
	for (size_t i = 0; i < token_count; ++i) {
		if (tokens[i]->tkn_type == t_constant) {
			
		} else {
			if (tokens[i]->tkn_val.nonc.val != NULL) {
				free(tokens[i]->tkn_val.nonc.val);
				tokens[i]->tkn_val.nonc.val = NULL;
			}
		}
		free(tokens[i]);
		tokens[i] = NULL;
	}
	free(tokens);
	tokens = NULL;
}

int esteh_lexical_parser() {
	tokens = (esteh_token **)malloc(ESTEH_TOKEN_FIRST_ALLOC);
	uint32_t lineno = 0;

	for (size_t i = 0; i < fmap_size; ++i) {
		
		if (fmap[i] == '\n') lineno++;

		if (fmap[i] == '/') {
			// Parse multiline comment.
			if (fmap[i + 1] == '*') {
				i++;
				while ((i < fmap_size) && (!(fmap[i] == '*' && fmap[i + 1] == '/'))) {
					i++;
					if (fmap[i] == '\n') lineno++;
				}
				i++;
				continue;
			}

			// Parse singleline comment.
			if (fmap[i + 1] == '/') {
				i++;
				while ((i < fmap_size) && (fmap[i] != '\n')) i++;
				lineno++;
				continue;
			}
		}

		if (
			(fmap[i] >= 'a' && fmap[i] <= 'z') || 
			(fmap[i] >= 'A' && fmap[i] <= 'Z') ||
			(fmap[i] == '_' || fmap[i] >= 128)
		) {
			size_t token_start = i++;
			while (
				(i < fmap_size) && 
				(
					(fmap[i] >= 'a' && fmap[i] <= 'z') || 
					(fmap[i] >= 'A' && fmap[i] <= 'Z') ||
					(fmap[i] >= '0' && fmap[i] <= '9') ||
					(fmap[i] == '_' || fmap[i] >= 128)
				)
			) i++;
			ESTEH_TOKEN_REALLOC
			tokens[token_count] = (esteh_token *)malloc(sizeof(esteh_token));
			tokens[token_count]->lineno = lineno;
			tokens[token_count]->tkn_val.nonc.len = i - token_start;
			tokens[token_count]->tkn_val.nonc.val = (char *)malloc(tokens[token_count]->tkn_val.nonc.len);
			memcpy(tokens[token_count]->tkn_val.nonc.val, fmap + token_start, tokens[token_count]->tkn_val.nonc.len);
			token_count++;
		}

	//t_number_parser:
		if (fmap[i] >= '0' && fmap[i] <= '9') {

			//
			// TODO: Parse hexadecimal, octal and binary.
			//

			// ...
			// ...
			// ...


			// Normal format for integer and float.
			size_t token_start = i++;
			bool is_float = false;
			while ((i < fmap_size) && ((fmap[i] >= '0' && fmap[i] <= '9') || fmap[i] == '.')) {
				if (fmap[i] == '.') {
					if (is_float) {
						PARSE_ERROR(
							"syntax error, unexpected '.', in \"%s\" on line %d",
							filename,
							lineno
						);
						exit(254);
					}
					is_float = true;
				}
				i++;
			}

			ESTEH_TOKEN_REALLOC
			tokens[token_count] = (esteh_token *)malloc(sizeof(esteh_token));
			tokens[token_count]->tkn_code = T_NUMBER;
			tokens[token_count]->tkn_type = t_constant;
			tokens[token_count]->lineno = lineno;
			char *tmp = strndup(fmap + token_start, i - token_start);
			if (is_float) {
				tokens[token_count]->tkn_val.data.type = tea_float;
				// TODO: Parse floating number.
				// Casting to double for now. Have to fix this in the future.
				tokens[token_count]->tkn_val.data.val.fval = (double)atol(tmp);
			} else {
				tokens[token_count]->tkn_val.data.type = tea_integer;
				tokens[token_count]->tkn_val.data.val.llval = atol(tmp);
			}
			free(tmp);
			token_count++;
		}

	// 	if (fmap[i] == '+') {
	// 		if (token_count > 0 && (tokens[token_count - 1]->tkn_code != T_NUMBER)) {

	// 		}
	// 	}

	}

	printf("Line: %d\n", lineno);

	return 0;
}

void token_analyze(char *tkn, esteh_token **token) {

	if (SCMP(tkn, "p") || SCMP(tkn, "print") || SCMP(tkn, "echo")) {
		(*token)->tkn_code = T_PRINT;
		(*token)->tkn_type = t_keyword;
		return;
	}


	return;
};
