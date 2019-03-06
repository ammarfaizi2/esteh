
#ifndef ESTEH_ARGV_PARSER_ARGV_PARSER_H
#define ESTEH_ARGV_PARSER_ARGV_PARSER_H

#include <stdbool.h>

typedef struct _argv_opt {

} argv_opt;

void esteh_argv_parser(char **argv, argv_opt ***opts, bool *is_error, char **error_message, char **filename);

#endif
