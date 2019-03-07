
#ifndef ESTEH_VM_BUFFER_H
#define ESTEH_VM_BUFFER_H

#define DEFAULT_BUFFER_SIZE (5 * sizeof(char))

#include <stdio.h>
#include <stdlib.h>

void esteh_add_buffer(char *buf, size_t len);
void esteh_flush_buffer();

#endif
