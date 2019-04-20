
#include <stdio.h>
#include <stdarg.h>
#include <esteh/vm/buffer/helpers.h>

void esteh_printf(char *format, ...) {
	char buffer[100];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	add_stdout_buffer(buffer, strlen(buffer));
}
