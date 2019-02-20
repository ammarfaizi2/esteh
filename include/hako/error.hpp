
#ifndef HAKO_ERROR_HPP
#define HAKO_ERROR_HPP

#include <cstdio>
#include <stdarg.h>
#include <stdlib.h>

void _hako_error(int exit_code, const char *msg, ...);
void hako_error(const char *msg, ...);

#endif
