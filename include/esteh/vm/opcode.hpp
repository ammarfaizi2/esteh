
#ifndef ESTEH_TOKEN_HPP
#define ESTEH_TOKEN_HPP

#include <stdint.h>

typedef struct _esteh_opcode {
	int code;
	size_t line;
	char *content;
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
#define TD_ECHO 0xd001

#define TE_STRING 0xe001

#endif