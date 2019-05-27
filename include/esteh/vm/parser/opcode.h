
#ifndef ESTEH__VM__PARSER__OPCODE_H
#define ESTEH__VM__PARSER__OPCODE_H


enum _op_type {
	t_constant = (1 << 0),
	t_variable = (1 << 1)
};

union op_dtype {
	
};

typedef struct _tea_op {

	enum _op_type op_type;
} tea_op;

typedef struct _tea_opcode {
	void *handler;
	uint32_t lineno;
	tea_op op1;
	tea_op op2;
	tea_op result;
} tea_opcode;

#endif
