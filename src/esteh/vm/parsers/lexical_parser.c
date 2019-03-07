
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
	for (uint32_t i = 0; i < token_count; ++i) {
		if (tokens[i]->tkn_type == t_constant) {
			if (tokens[i]->tkn_val.data.type == TEA_STRING) {
				free(tokens[i]->tkn_val.data.val.str.val);
				tokens[i]->tkn_val.data.val.str.val = NULL;
			}
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

inline static void escape_char();

int esteh_lexical_parser() {
	tokens = (esteh_token **)malloc(ESTEH_TOKEN_FIRST_ALLOC);
	uint32_t lineno = 0;
	for (size_t i = 0; i < fmap_size; ++i) {
		
		if (fmap[i] == ';') {
			ESTEH_TOKEN_REALLOC
			tokens[token_count] = (esteh_token *)malloc(sizeof(esteh_token));
			tokens[token_count]->tkn_code = T_SEMICOLON;
			tokens[token_count]->tkn_type = t_semicolon;
			tokens[token_count]->lineno = lineno;
			tokens[token_count]->tkn_val.nonc.val = NULL;
			token_count++;
			continue;
		}


		bool whitespace_go_back = false;
		if (fmap[i] == '\n') lineno++;
		if (fmap[i] == '\n' || fmap[i] == ' ' || fmap[i] == '\r' || fmap[i] == '\t') {
			i++;
			while ((i < fmap_size) && (fmap[i] == '\n' || fmap[i] == ' ' || fmap[i] == '\r' || fmap[i] == '\t')) i++;
			if (fmap[i] == '/') {
				whitespace_go_back = true;
				goto comment_parser;
			}

whitespace_parser:
	
			// Skip save first whitespace.
			if (token_count > 0) {
				ESTEH_TOKEN_REALLOC
				tokens[token_count] = (esteh_token *)malloc(sizeof(esteh_token));
				tokens[token_count]->tkn_code = T_WHITESPACE;
				tokens[token_count]->tkn_type = t_whitespace;
				tokens[token_count]->lineno = lineno;
				tokens[token_count]->tkn_val.nonc.val = NULL;
				token_count++;
			}
		}

		if (fmap[i] == '/') {
comment_parser:
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

			if (whitespace_go_back) {
				goto whitespace_parser;
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
			token_analyze(fmap + token_start, i - token_start, &tokens[token_count]);
			token_count++;
			i--;
		}

		if (fmap[i] == '"') {
			bool escaped = false;
			#define TMP_STR_FIRST_ALLOC 30;
			size_t cur_stralloc = TMP_STR_FIRST_ALLOC;
			char *tmp = (char *)malloc(cur_stralloc);
			size_t cur_strlen = 0;
			i++;
			while (fmap[i] != '"') {

				if ((cur_strlen + 2) > cur_stralloc) {
					cur_stralloc += TMP_STR_FIRST_ALLOC;
					tmp = (char *)realloc(tmp, cur_stralloc);
					continue;
				}

				if (i >= fmap_size) {
					PARSE_ERROR(
						"syntax error, unexpected end of file, in \"%s\" on line %d",
						filename,
						lineno
					);
					
					exit(254);
				} else {
					if (fmap[i] == '\n') lineno++;
					if (escaped) {
						escape_char(&cur_strlen, &tmp, fmap, &i);
						escaped = false;
						continue;
					}
					if (fmap[i] == '\\') {
						escaped = true;
						i++;
						continue;
					}
					tmp[cur_strlen] = fmap[i];
					cur_strlen++;
					i++;
				}

			}

			ESTEH_TOKEN_REALLOC
			tokens[token_count] = (esteh_token *)malloc(sizeof(esteh_token));
			tokens[token_count]->tkn_code = T_STRING;
			tokens[token_count]->tkn_type = t_constant;
			tokens[token_count]->lineno = lineno;
			tokens[token_count]->tkn_val.data.type = TEA_STRING;
			tokens[token_count]->tkn_val.data.val.str.len = cur_strlen;
			tokens[token_count]->tkn_val.data.val.str.val = (char *)malloc(tokens[token_count]->tkn_val.data.val.str.len);
			memcpy(tokens[token_count]->tkn_val.data.val.str.val, tmp, tokens[token_count]->tkn_val.data.val.str.len);
			free(tmp);
			tmp = NULL;
			token_count++;
		}

		bool is_negative = false;
		bool maybe_num_symbol = false;

t_number_parser:
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
				if (is_negative) {
					tokens[token_count]->tkn_val.data.val.fval *= -1;					
				}
			} else {
				tokens[token_count]->tkn_val.data.type = tea_integer;
				tokens[token_count]->tkn_val.data.val.llval = atol(tmp);
				if (is_negative) {
					tokens[token_count]->tkn_val.data.val.llval *= -1;					
				}
			}
			free(tmp);
			tmp = NULL;
			i--;
			token_count++;
			continue;
		}

		if (fmap[i] == '+') {
			if (token_count > 0) {
				ESTEH_TOKEN_REALLOC
				tokens[token_count] = (esteh_token *)malloc(sizeof(esteh_token));
			}
			tokens[token_count]->tkn_code = T_OP_ADD;
			tokens[token_count]->tkn_type = t_operator;
			tokens[token_count]->lineno = lineno;
			tokens[token_count]->tkn_val.nonc.val = NULL;
			token_count++;
			continue;
		}

		if (fmap[i] == '-') {
			if (token_count > 0) {
				if (tokens[token_count - 1]->tkn_type != t_constant) {
					is_negative = true;
					maybe_num_symbol = true;
					i++;
					goto t_number_parser;
					continue;
				}
			}
		}

	}

	// If the latest token is a T_WHITESPACE, then ignore it.
	if (tokens[token_count - 1]->tkn_type == t_whitespace) {
		free(tokens[token_count - 1]);
		tokens[token_count - 1] = NULL;
		token_count--;
	}

	for (uint32_t i = 0; i < token_count; ++i)
	{
		TOKEN_DUMPER(tokens[i]);
	}


	return 0;
}

inline static void escape_char(size_t *cur_strlen, char **tmp, char *fmap, size_t *pos) {

	#define DMQQ(A,B) \
		case A: \
			(*tmp)[*cur_strlen] = B; \
			(*cur_strlen)++; \
			(*pos)++; \

	switch (fmap[*pos]) {
		DMQQ('n', '\n')
			return;
		break;
		DMQQ('t', '\t')
			return;
		break;
		DMQQ('f', '\f')
			return;
		break;
		DMQQ('r', '\r')
			return;
		break;
		DMQQ('v', '\v')
			return;
		break;
		DMQQ('e', '\e')
			return;
		break;
		DMQQ('a', '\a')
			return;
		break;
		DMQQ('b', '\b')
			return;
		break;

		#if 0
		case 'x':
			// TODO: Parse hexadecimal char format.
		break;
		default:
			if (fmap[*pos] >= '0' && fmap[*pos] <= '7') {
				// TODO: Parse octal char format.
			}
		break;
		#endif
	}
}

void token_analyze(char *tkn, size_t tll, esteh_token **token) {

	if (SNCMP(tkn, "p", tll) || SNCMP(tkn, "print", tll) || SNCMP(tkn, "echo", tll)) {
		(*token)->tkn_val.nonc.val = NULL;
		(*token)->tkn_code = T_PRINT;
		(*token)->tkn_type = t_keyword;
		return;
	}

	(*token)->tkn_val.nonc.val = (char *)malloc(tll);
	(*token)->tkn_code = T_NOT_DEFINED_YET;
	(*token)->tkn_type = t_symbol;
	memcpy((*token)->tkn_val.nonc.val, tkn, tll);
	return;
}
