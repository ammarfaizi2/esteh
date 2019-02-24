
#ifndef ESTEH_TOKEN_HPP
#define ESTEH_TOKEN_HPP

#include <stdint.h>
#include <stdlib.h>

typedef struct _tehnode_op {
	
} tehnode_op;

typedef struct _esteh_opcode {
	int code;
	size_t line;
	void *content;
} esteh_opcode;

typedef struct _opcode_sv {
	int code;
	uint32_t line;
	int c_start;
	size_t c_size;
} opcode_sv;

typedef struct _opcache_header {
	size_t opcodes_amount;
} opcache_header;

#define T_UNKNOWN -1

#define TC_NOTHING 100

#define TD_PRINT 0b000000000

#define TE_STRING 300
#define TE_INT 301

#define TE_ADD 1 // +
#define TE_MIN 2 // -
#define TE_MUL 4 // *
#define TE_DIV 8 // /

#endif