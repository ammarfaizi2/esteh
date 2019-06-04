
#include <string.h>
#include <unistd.h>
#include <estehvm/vm/stdio.h>

char *esteh_stderr_buffer;
size_t stderr_stored_buffer_size = 0;
size_t esteh_stderr_buffer_size = 8092;

void esteh_stderr_buffer_init() {
	esteh_stderr_buffer = (char *)malloc(esteh_stderr_buffer_size + 1);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

void esteh_stderr_buffer_flush() {
	if (stderr_stored_buffer_size > 0) {
		write(2, esteh_stderr_buffer, stderr_stored_buffer_size);
		stderr_stored_buffer_size = 0;
	}
}

void esteh_stderr_buffer_add(char *buff, size_t size) {
	size_t free_size = esteh_stderr_buffer_size - stderr_stored_buffer_size;
	if (size >= free_size) {
		esteh_stderr_buffer_flush();
		write(2, buff, free_size);
		buff += free_size;
		size -= free_size;
		if (size >= esteh_stderr_buffer_size) {
			size_t buff_cycle = size/esteh_stderr_buffer_size;
			for (size_t i = 0; i < buff_cycle; ++i) {
				write(2, buff, esteh_stderr_buffer_size);
				buff += esteh_stderr_buffer_size;
				size -= esteh_stderr_buffer_size;
			}
		}
		memcpy(esteh_stderr_buffer, buff, size);
		stderr_stored_buffer_size += size;
	} else {
		memcpy(esteh_stderr_buffer + stderr_stored_buffer_size, buff, size);
		stderr_stored_buffer_size += size;
	}
}

#pragma GCC diagnostic pop
