
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <esteh/vm/buffer.h>

char *buffer;
size_t buffer_size;
bool buffer_is_not_initialized_yet = true;
size_t output_buffer_size_alloc = DEFAULT_BUFFER_SIZE;

static inline void esteh_init_buffer() {
	buffer = malloc(output_buffer_size_alloc);
	buffer_is_not_initialized_yet = false;
}

void esteh_add_buffer(char *buf, size_t len) {
	if (buffer_is_not_initialized_yet) {
		esteh_init_buffer();
		if (len >= output_buffer_size_alloc) {
			write(1, buf, len);
			return;
		}
		goto buf_copy;
	}

	if ((buffer_size + len) >= output_buffer_size_alloc) {
		esteh_flush_buffer();

		if (len >= output_buffer_size_alloc) {
			write(1, buf, len);
			return;
		}
	}

buf_copy:
	memcpy(buffer, buf, len);
	buffer_size += len;
	return;
}

void esteh_flush_buffer() {
	if (buffer_size > 0) {
		write(1, buffer, buffer_size);
		buffer_size = 0;
	}
}

