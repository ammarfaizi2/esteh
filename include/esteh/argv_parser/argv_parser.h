
#ifndef ESTEH_ARGV_PARSER_ARGV_PARSER_H
#define ESTEH_ARGV_PARSER_ARGV_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include <esteh/argv_parser/argv_opt.h>

uint16_t esteh_argv_parser(
	int argc,
	char **argv,
	argv_opt ***opts,
	char **filename,
	char ***app_argv,
	int *app_argc
);

#endif
