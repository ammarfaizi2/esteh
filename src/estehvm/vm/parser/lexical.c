
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <estehvm/vm/esteh.h>
#include <estehvm/vm/token.h>
#include <estehvm/vm/debugger/token.h>

uint32_t esteh_vm_lexical_analyze(char *fmap, size_t fsize, esteh_token ***tokens, bool *is_error, char **error_message) {
	uint32_t token_count = 0;
	uint32_t lineno = 1;
	#define $t (*tokens)
	#define $c (fmap)
	#define $ti token_count

	#define PARSE_ERROR(...) \

	#define SET_TOKEN(TOKEN_TYPE, TOKEN_BODY, TOKEN_BODY_SIZE, TOKEN_LINENO) \
		$t[$ti] = (esteh_token *)malloc(sizeof(esteh_token)); \
		$t[$ti]->type = TOKEN_TYPE; \
		$t[$ti]->body_size = TOKEN_BODY_SIZE; \
		$t[$ti]->body = (char *)malloc(TOKEN_BODY_SIZE); \
		$t[$ti]->lineno = TOKEN_LINENO; \
		memcpy($t[$ti]->body, TOKEN_BODY, TOKEN_BODY_SIZE); \
		$ti++;

	#define SET_TOKEN_NP(TOKEN_TYPE, TOKEN_BODY, TOKEN_BODY_SIZE, TOKEN_LINENO) \
		$t[$ti] = (esteh_token *)malloc(sizeof(esteh_token)); \
		$t[$ti]->type = TOKEN_TYPE; \
		$t[$ti]->body_size = TOKEN_BODY_SIZE; \
		$t[$ti]->body = TOKEN_BODY; \
		$t[$ti]->lineno = TOKEN_LINENO; \
		$ti++;

	$t = (esteh_token **)malloc(ESTEH_TOKEN_SGGT * (sizeof(esteh_token *)));

	for (uint32_t i = 0; i < fsize; i++) {

		#include "components/whitespace_parser.h"
		#include "components/comment_parser.h"
		#include "components/string_parser.h"
		#include "components/number_parser.h"

	}

	TOKEN_DUMPER($t, token_count);

	#undef $t
	#undef $c
	#undef $ti
	#undef SET_TOKEN
	#undef SET_TOKEN_NP

	printf("Token count: %d\n", token_count);

	return token_count;
}
