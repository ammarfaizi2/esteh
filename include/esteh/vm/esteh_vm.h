
#ifndef ESTEH_VM_ESTEH_VM_H
#define ESTEH_VM_ESTEH_VM_H

#include <stdint.h>
#include <esteh/vm/esteh_token.h>
#include <esteh/argv_parser/argv_opt.h>

int esteh_vm(
	char *filename,
	int opt_count,
	argv_opt **opts,
	int argc,
	char **argv
);
int esteh_vm_parse_file(char *filename);
int esteh_lexical_parser();
void token_analyze(char *tkn, esteh_token **token);
void esteh_token_clean_up();
void esteh_vm_shutdown();
void esteh_vm_end_shutdown();

#endif
