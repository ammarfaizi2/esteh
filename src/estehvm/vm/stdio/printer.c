
#include <stdio.h>
#include <stdarg.h>
#include <estehvm/vm/stdio.h>

size_t esteh_printf(char *format, ...) {
	va_list args;
	size_t needed_size = 0;
	char *buff;

	va_start(args, format);
	needed_size = vsnprintf(NULL, 0, format, args) + 1;
	buff = (char *)malloc(needed_size + 1);
	va_end(args);

	va_start(args, format);
	vsnprintf(buff, needed_size, format, args);
	esteh_stdout_buffer_add(buff, needed_size - 1);
	free(buff);
	va_end(args);

	return needed_size;
}

size_t esteh_err_printf(char *format, ...) {
	va_list args;
	size_t needed_size = 0;
	char *buff;

	va_start(args, format);
	needed_size = vsnprintf(NULL, 0, format, args);
	buff = (char *)malloc(needed_size + 1);
	va_end(args);

	va_start(args, format);
	vsnprintf(buff, needed_size, format, args);
	esteh_stderr_buffer_add(buff, needed_size - 1);
	free(buff);
	va_end(args);

	return needed_size;
}
