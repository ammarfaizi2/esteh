
#ifndef ESTEH_ERROR_HPP
#define ESTEH_ERROR_HPP

#include <cstdio>
#include <stdarg.h>
#include <stdlib.h>

void _esteh_error(int exit_code, const char *msg, ...);
void esteh_error(const char *msg, ...);

#endif
