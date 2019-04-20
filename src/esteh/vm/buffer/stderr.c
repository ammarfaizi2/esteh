
#include <esteh/vm/buffer/buffer.h>

char *stderr_buffer = NULL;
size_t stderr_buffer_ptr = 0;
size_t stderr_buffer_size = 8192;

void init_stderr_buffer() {
	stderr_buffer = (char *)malloc(stderr_buffer_size + 1);
}

void flush_stderr_buffer() {
	write(2, stderr_buffer, stderr_buffer_ptr);
	stderr_buffer_ptr = 0;
}

void add_stderr_buffer(char *data, size_t size) {
	size_t free_buffer_size = stderr_buffer_size - stderr_buffer_ptr;
	if (size >= free_buffer_size) {
		memcpy(stderr_buffer+stderr_buffer_ptr, data, free_buffer_size);
		stderr_buffer_ptr += free_buffer_size;
		flush_stderr_buffer();
		size_t rest_size = size - free_buffer_size;
		while (rest_size >= stderr_buffer_size) {
			memcpy(stderr_buffer, data+size-rest_size, stderr_buffer_size);
			stderr_buffer_ptr += stderr_buffer_size;
			rest_size -= stderr_buffer_size;
			flush_stderr_buffer();
		}
		if (rest_size > 0) {
			stderr_buffer_ptr = rest_size;
			memcpy(stderr_buffer, data+size-rest_size, rest_size);
		}
	} else {
		memcpy(stderr_buffer+stderr_buffer_ptr, data, size);
		stderr_buffer_ptr += size;
	}
}

void clean_up_stderr_buffer() {
	free(stderr_buffer);
}