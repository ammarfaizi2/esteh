
#ifndef ESTEH_VM_PARSERS_PARSE_ERROR_H
#define ESTEH_VM_PARSERS_PARSE_ERROR_H

#include <stdio.h>

#define PARSE_ERROR(FORMAT, ...) fprintf(stdout, "EsTeh Parse Error: " FORMAT "\n", __VA_ARGS__)

#endif
