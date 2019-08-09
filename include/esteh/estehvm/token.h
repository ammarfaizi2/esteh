
#ifndef __esteh__estehvm__token_h
#define __esteh__estehvm__token_h

#include <stdlib.h>
#include <stdint.h>

typedef enum {
	t_any = (1 << 0),
	t_string = (1 << 1)
} token_type;

typedef struct esteh_token_ {
	token_type type;
	size_t body_size;
	char body[1];
} esteh_token;

uint32_t esteh_vm_lexical_analyze(char *map, size_t map_size, esteh_token ***tokens);

#endif
