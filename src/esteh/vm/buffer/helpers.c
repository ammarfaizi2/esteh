
#include <stdio.h>
#include <stdarg.h>
#include <esteh/vm/buffer/buffer.h>
#include <esteh/vm/buffer/helpers.h>

size_t esteh_printf(const char *format, ...) {
    va_list args;
    size_t needed; 
    char *buffer;

    va_start(args, format);
    needed = vsnprintf(NULL, 0, format, args) + 1;
    buffer = (char *)malloc(needed);
    va_end(args);

    va_start(args, format);
    needed = vsprintf(buffer, format, args);
    add_stdout_buffer(buffer, needed);
    va_end(args);

    free(buffer);
    buffer = NULL;

    return needed;
}
