
#ifndef ESTEH_PRINT_HPP
#define ESTEH_PRINT_HPP

#include <string.h>
#include <esteh/vm/opcode.hpp>
#include <esteh/vm/esteh_buffer.hpp>

void esteh_print(teaval val) {
	if (val.type == ESTEH_TYPE_STRING) {
		esteh_buffer_add(val.value.str.val, val.value.str.len, 1);
	} else if (val.type == ESTEH_TYPE_INT) {
		esteh_buffer_add(val.value.str.val, val.value.str.len, 1);
	}
}

#endif
