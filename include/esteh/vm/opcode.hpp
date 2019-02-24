
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
#define TD_PRINT 100

#define TE_STRING 200
#define TE_INT 201

#define TE_ADD 210 // +
#define TE_MIN 211 // -
#define TE_MUL 212 // *
#define TE_DIV 213 // /

#endif