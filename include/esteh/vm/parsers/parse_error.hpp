
#ifndef ESTEH_VM_PARSERS_PARSE_ERROR_HPP
#define ESTEH_VM_PARSERS_PARSE_ERROR_HPP

#include <cstdio>

#define PARSE_ERROR(FORMAT, ...) fprintf(stdout, "EsTeh Parse Error: " FORMAT "\n", __VA_ARGS__)

#endif
