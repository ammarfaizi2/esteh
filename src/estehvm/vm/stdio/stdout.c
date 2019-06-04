
#include <string.h>
#include <unistd.h>
#include <estehvm/vm/stdio.h>

char *esteh_stdout_buffer;
size_t stdout_stored_buffer_size = 0;
size_t esteh_stdout_buffer_size = 8092;

void esteh_stdout_buffer_init() {
	esteh_stdout_buffer = (char *)malloc(esteh_stdout_buffer_size + 1);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

void esteh_stdout_buffer_flush() {
	if (stdout_stored_buffer_size > 0) {
		write(1, esteh_stdout_buffer, stdout_stored_buffer_size);
		stdout_stored_buffer_size = 0;
	}
}

void esteh_stdout_buffer_add(char *buff, size_t size) {
	size_t free_size = esteh_stdout_buffer_size - stdout_stored_buffer_size;
	if (size >= free_size) {
		esteh_stdout_buffer_flush();
		write(1, buff, free_size);
		buff += free_size;
		size -= free_size;
		if (size >= esteh_stdout_buffer_size) {
			size_t buff_cycle = size/esteh_stdout_buffer_size;
			for (size_t i = 0; i < buff_cycle; ++i) {
				write(1, buff, esteh_stdout_buffer_size);
				buff += esteh_stdout_buffer_size;
				size -= esteh_stdout_buffer_size;
			}
		}
		memcpy(esteh_stdout_buffer, buff, size);
		stdout_stored_buffer_size += size;
	} else {
		memcpy(esteh_stdout_buffer + stdout_stored_buffer_size, buff, size);
		stdout_stored_buffer_size += size;
	}
}

#pragma GCC diagnostic pop
