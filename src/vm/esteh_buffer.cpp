
#include <cstdio>
#include <string.h>
#include <unistd.h>
#include <esteh/vm/esteh_buffer.hpp>

#define _ESTEH_BUFFER_SIZE 8192
#define ESTEH_BUFFER_SIZE (_ESTEH_BUFFER_SIZE - 1)

#define FLUSH_BUFFER \
	write(_fd, __esteh_buffer, allocated_buf); \
	allocated_buf = 0;

size_t allocated_buf = 0;
char *__esteh_buffer = (char *)malloc(ESTEH_BUFFER_SIZE + 2);

void esteh_buffer_add(char *str, size_t len, uint8_t _fd = 1) {	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
	if (allocated_buf < ESTEH_BUFFER_SIZE) {

		for (size_t k = 0; (allocated_buf <= ESTEH_BUFFER_SIZE) && (k < len);) {
			__esteh_buffer[allocated_buf] = str[k];

			if (allocated_buf >= ESTEH_BUFFER_SIZE) {
				FLUSH_BUFFER
			} else {
				k++;
				allocated_buf++;
			}
		}

		if (allocated_buf >= ESTEH_BUFFER_SIZE) {
			FLUSH_BUFFER
		}
	}	
}

void buffer_shutdown(uint8_t _fd = 1) {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
	if (allocated_buf > 0) {
		FLUSH_BUFFER
	}
	#pragma GCC diagnostic pop
}
