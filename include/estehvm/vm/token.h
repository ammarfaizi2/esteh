
#ifndef estehvm__vm__token_h
#define estehvm__vm__token_h

#include <stdint.h>
#include <estehvm/vm/token.h>

typedef enum {
	t_whitespace = 1,
	t_string = 2,
	t_number = 3,
	t_symbol = 4,
	t_constant = 5
} esteh_token_type;

typedef struct _esteh_token {
	char *body;
	size_t body_size;
	uint32_t lineno;
	esteh_token_type type;
} esteh_token;

#define ESTEH_TOKEN_SGGT 1024

uint32_t esteh_vm_lexical_analyze(char *fmap, size_t fsize, esteh_token ***tokens);

#endif
