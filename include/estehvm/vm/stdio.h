
#ifndef estehvm__vm__stdio_h
#define estehvm__vm__stdio_h

#include <stdlib.h>

size_t esteh_printf(char *format, ...);
size_t esteh_err_printf(char *format, ...);
void esteh_stdout_buffer_init();
void esteh_stdout_buffer_flush();
void esteh_stdout_buffer_add(char *buff, size_t size);
void esteh_stderr_buffer_init();
void esteh_stderr_buffer_flush();
void esteh_stderr_buffer_add(char *buff, size_t size);

#endif
