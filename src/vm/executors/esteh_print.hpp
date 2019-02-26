
#ifndef ESTEH_PRINT_HPP
#define ESTEH_PRINT_HPP

#include <math.h>
#include <cstdio>
#include <limits.h>
#include <string.h>
#include <esteh/vm/opcode.hpp>
#include <esteh/vm/esteh_buffer.hpp>

static inline char* lltoa(long long val, int base){

    static char buf[64] = {0};

    int i = 62;
    int sign = (val < 0);
    if (sign) val = -val;
    
    if (val == 0) {
    	buf[0] = '0';
    	buf[1] = '\0';
    	return buf;
    }

    for (; val && i ; --i, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }

    if (sign) {
        buf[i--] = '-';
    }

    return &buf[i+1];
}

void esteh_print(teaval val) {
	if (val.type == ESTEH_TYPE_STRING) {
		esteh_buffer_add(val.value.str.val, val.value.str.len, 1);
	} else if (val.type == ESTEH_TYPE_INT) {
		char *buf = lltoa(val.value.lval, 10);
		esteh_buffer_add(buf, floor(log10(val.value.lval)) + 1, 1);
	}
}

#endif
