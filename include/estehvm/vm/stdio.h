
#ifndef estehvm__vm__stdio_h
#define estehvm__vm__stdio_h

#include <stdlib.h>

void esteh_stdout_buffer_init();
void esteh_stdout_buffer_flush();
void esteh_stdout_buffer_add(char *buff, size_t size);

#endif
