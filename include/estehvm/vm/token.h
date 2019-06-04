
#ifndef estehvm__vm__token_h
#define estehvm__vm__token_h

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
	t_whitespace = 1,
	t_string = 2,
	t_number = 3,
	t_symbol = 4,
	t_operator = 5
} esteh_token_type;

typedef struct _esteh_token {
	char *body;
	size_t body_size;
	uint32_t lineno;
	esteh_token_type type;
} esteh_token;

//
// Intermediate bytecode
//

typedef enum _esteh_bytecode_type {
	tb_print = 1,
	tb_echo = 2,
	tb_constant = 3,
	tb_variable = 4,
} esteh_bytecode_type;

typedef union _esteh_bytecode_data {

} bytecode_data;

typedef struct _esteh_bytecode {
	bytecode_data data;
	esteh_bytecode_type type;
} esteh_bytecode;







#define ESTEH_TOKEN_SGGT 1024
uint32_t esteh_vm_lexical_analyze(char *fmap, size_t fsize, esteh_token ***tokens, bool *is_error, char **error_message);

#endif
