
#ifndef ESTEH_BUFFER_HPP
#define ESTEH_BUFFER_HPP

#include <stdint.h>
#include <stdlib.h>

void esteh_ob_start();
void esteh_buffer_add(char *, size_t, uint8_t);
void esteh_ob_end();
void buffer_shutdown(uint8_t);

#endif
