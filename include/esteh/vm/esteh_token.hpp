
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

typedef struct _tea_node tea_node;
typedef struct _esteh_token esteh_token;

union tea_val {
	tea_node *node;
	struct {
		union esteh_val val;
		uint16_t data_type;
	} constant;
};

enum tea_node_type {
	an_unknown,
	a_constant,
	a_tea_node,
};

struct _tea_node {
	void *handler;
	tea_val op1;
	tea_val op2;
	tea_val result;
	uint16_t token = 0;
	tea_node_type op1_type;
	tea_node_type op2_type;
	tea_node_type result_type;
};

enum token_type {
	t_end_of_file,
	t_symbol,
	t_keyword,
	t_operator,
	t_constant,
	t_unknown = -1
};

struct _esteh_token {
	uint16_t token = 0;
	uint32_t line;
	enum token_type type = t_unknown;
	esteh_val val;
	uint16_t val_type = 0;
};

#define ESTEH_TOKEN_PTR_FIRST_ALLOC	(sizeof(esteh_token) * 30)
#define ESTEH_TOKEN_FIRST_ALLOC		(sizeof(char) * 300)
#define TEA_NODE_FIRST_ALLOC		(sizeof(tea_node) * 10)

#define TA_ADD (1 << 0)
#define TA_MIN (1 << 1)
#define TA_MUL (1 << 2)
#define TA_DIV (1 << 3)
#define TA_MOD (1 << 4)

#define T_INT	 	100
#define T_STRING 	101

#define T_PRINT	 	200

#define T_SEMICOLON 300
#endif
