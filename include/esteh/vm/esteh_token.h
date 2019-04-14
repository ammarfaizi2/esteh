

#ifndef ESTEH_VM_ESTEH_TOKEN_H
#define ESTEH_VM_ESTEH_TOKEN_H

#include <stdint.h>
#include <esteh/vm/data_types.h>

#define ESTEH_TOKEN_FIRST_ALLOC (sizeof(esteh_token *) * 50)

#define T_NOT_DEFINED_YET 0
#define T_PRINT 100
#define T_SEMICOLON 130
#define T_WHITESPACE 150

#define T_NULL 200
#define T_NUMBER 201
#define T_STRING 202

#define T_OP_ADD 300
#define T_OP_MIN 301
#define T_OP_DIV 302
#define T_OP_MUL 303
#define T_OP_MOD 304
#define T_OP_BOOL_NOT 305
#define T_OP_LESS_THAN 306
#define T_OP_GREATER_THAN 307

enum token_type {
	t_unknown = 0,
	t_keyword = (1 << 0),
	t_symbol = (1 << 1),
	t_constant = (1 << 2),
	t_operator = (1 << 3),
	t_whitespace = (1 << 4),
	t_semicolon = (1 << 5)
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
