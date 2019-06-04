
#include <stdlib.h>
#include <string.h>
#include <estehvm/vm/esteh.h>
#include <estehvm/vm/token.h>

uint32_t esteh_vm_lexical_analyze(char *fmap, size_t fsize, esteh_token ***tokens) {
	uint32_t token_count = 0;
	uint32_t lineno = 1;
	#define $t (*tokens)
	#define $c (fmap)
	#define $ti token_count

	#define SET_TOKEN(TOKEN_TYPE, TOKEN_BODY, TOKEN_BODY_SIZE, TOKEN_LINENO) \
		$t[$ti] = (esteh_token *)malloc(sizeof(esteh_token)); \
		$t[$ti]->type = TOKEN_TYPE; \
		$t[$ti]->body_size = TOKEN_BODY_SIZE; \
		$t[$ti]->body = (char *)malloc(TOKEN_BODY_SIZE); \
		$t[$ti]->lineno = TOKEN_LINENO; \
		memcpy($t[$ti]->body, TOKEN_BODY, TOKEN_BODY_SIZE);

	$t = (esteh_token **)malloc(ESTEH_TOKEN_SGGT * (sizeof(esteh_token *)));

	for (uint32_t i = 0; i < fsize; i++) {

		#include "components/comment_parser.cxx"

	}

	#undef $c
	#undef $t
	#undef $ti
	#undef SET_TOKEN

	printf("Token count: %d\n", token_count);

	return token_count;
}
