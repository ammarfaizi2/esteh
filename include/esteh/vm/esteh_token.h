

#ifndef ESTEH_VM_TOKEN_H
#define ESTEH_VM_TOKEN_H

#include <stdint.h>

typedef struct _esteh_token {
	uint16_t tkn_code;
	struct {
		char *val;
		size_t len;
	} content;
	uint32_t lineno;
} esteh_token;

#endif
