
#ifndef __esteh__argv_parser_h
#define __esteh__argv_parser_h

#include <esteh/estehvm/estehvm.h>

#include <stdint.h>
#include <stdbool.h>

void usage(char *app_name);
bool esteh_argv_parser(
	uint8_t *exit_code,
	esteh_vm_rfile *vm_run_file,
	int argc,
	char **argv,
	char **envp
);

#endif
