
#ifndef ESTEH_TOKEN_HPP
#define ESTEH_TOKEN_HPP

typedef struct _esteh_opcode {
	int code;
	int line;
	char *content;
} esteh_opcode;

typedef struct _opcode_sv {
	int code;
	int line;
	int c_start;
	int c_size;
} opcode_sv;


#define T_UNKNOWN -1
#define TD_ECHO 0xd001

#define TE_STRING 0xe001

#endif