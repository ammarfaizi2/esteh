
#ifndef ESTEH_VM_ESTEH_TOKEN_HPP
#define ESTEH_VM_ESTEH_TOKEN_HPP

#include <stdint.h>
#include <stdlib.h>
#include <esteh/vm/data_types.hpp>

union esteh_val {
	long long lval;
	double fval;
	struct {
		char *val;
		size_t len;
	} str;
};

typedef struct _esteh_token esteh_token;

typedef struct _tea_val {
	esteh_token *token;
	union esteh_val constant;
} tea_val;

enum tea_node_type {
	a_constant,
	a_tea_node_op
};

typedef struct _tea_node_op {
	void *handler;
	tea_val op1;
	tea_val op2;
	tea_val result;
} tea_node_op;

enum token_type {
	t_any,
	t_keyword,
	t_identifier,
	t_operator,
	t_constant,
	t_symbol
};

struct _esteh_token {
	uint16_t token = 0;
	uint32_t line;
	enum token_type type;
	esteh_val val;
};

#define ESTEH_TOKEN_PTR_FIRST_ALLOC	(sizeof(esteh_token) * 30)
#define ESTEH_TOKEN_FIRST_ALLOC		(sizeof(char) * 300)

#define TA_ADD (1 << 0)
#define TA_MIN (1 << 1)
#define TA_MUL (1 << 2)
#define TA_DIV (1 << 3)

#define T_INT	 	100
#define T_STRING 	101

#define T_PRINT	 	200

#define T_SEMICOLON 300
#endif
