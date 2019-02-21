
#ifndef HAKO_TOKEN_HPP
#define HAKO_TOKEN_HPP

typedef struct _hako_opcode {
	int code;
	int line;
	char *content;
} hako_opcode;

#define T_UNKNOWN -1
#define TD_ECHO 0xd001

#define TE_STRING 0xe001

#endif
