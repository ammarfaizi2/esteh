
#ifndef ESTEH__VM__BUFFER__BUFFER_H
#define ESTEH__VM__BUFFER__BUFFER_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void init_stdout_buffer();
void init_stderr_buffer();
void add_stdout_buffer(char *data, size_t size);
void add_stderr_buffer(char *data, size_t size);
void flush_stdout_buffer();
void flush_stderr_buffer();

#endif
