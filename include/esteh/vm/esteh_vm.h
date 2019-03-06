
#ifndef ESTEH_VM_ESTEH_VM_H
#define ESTEH_VM_ESTEH_VM_H

#include <esteh/argv_parser/argv_opt.h>

int esteh_vm(
	char *filename,
	int opt_count,
	argv_opt **opts,
	int argc,
	char **argv
);
int esteh_vm_parse_file(char *filename);
void esteh_vm_shutdown();
void esteh_vm_end_shutdown();

#endif
