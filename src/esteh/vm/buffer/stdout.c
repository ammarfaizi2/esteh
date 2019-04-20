
#include <esteh/vm/buffer/buffer.h>

char *stdout_buffer = NULL;
size_t stdout_buffer_ptr = 0;
size_t stdout_buffer_size = 8192;

void init_stdout_buffer() {
	stdout_buffer = (char *)malloc(stdout_buffer_size + 1);
}

void flush_stdout_buffer() {
	write(1, stdout_buffer, stdout_buffer_ptr);
	stdout_buffer_ptr = 0;
}

void add_stdout_buffer(char *data, size_t size) {
	size_t free_buffer_size = stdout_buffer_size - stdout_buffer_ptr;
	if (size >= free_buffer_size) {
		memcpy(stdout_buffer+stdout_buffer_ptr, data, free_buffer_size);
		stdout_buffer_ptr += free_buffer_size;
		flush_stdout_buffer();
		size_t rest_size = size - free_buffer_size;
		while (rest_size >= stdout_buffer_size) {
			memcpy(stdout_buffer, data+size-rest_size, stdout_buffer_size);
			stdout_buffer_ptr += stdout_buffer_size;
			rest_size -= stdout_buffer_size;
			flush_stdout_buffer();
		}
		if (rest_size > 0) {
			stdout_buffer_ptr = rest_size;
			memcpy(stdout_buffer, data+size-rest_size, rest_size);
		}
	} else {
		memcpy(stdout_buffer+stdout_buffer_ptr, data, size);
		stdout_buffer_ptr += size;
	}
}

void clean_up_stdout_buffer() {
	free(stdout_buffer);
	stdout_buffer = NULL;
}