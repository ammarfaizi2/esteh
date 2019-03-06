

#ifndef ESTEH_VM_TOKEN_H
#define ESTEH_VM_TOKEN_H

#include <stdint.h>
#include <esteh/vm/data_types.h>

#define ESTEH_TOKEN_FIRST_ALLOC (sizeof(esteh_token *) * 50)

#define T_NOT_DEFINED_YET 0

#define T_PRINT 100
#define T_WHITESPACE 150

#define T_NULL 200
#define T_NUMBER 201
#define T_STRING 202

enum token_type {
	t_unknown = 0,
	t_keyword = (1 << 0),
	t_symbol = (1 << 1),
	t_constant = (1 << 2),
	t_operator = (1 << 3),
	t_whitespace = (1 << 4)
};

enum tea_keywords {
	t_print = T_PRINT
};

union token_val {
	struct {
		char *val;
		size_t len;
	} nonc;
	tea_data data;
};

typedef struct _esteh_token {
	uint16_t tkn_code;
	enum token_type tkn_type;
	union token_val tkn_val;
	uint32_t lineno;
} esteh_token;

#endif
