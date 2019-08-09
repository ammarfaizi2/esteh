
#ifndef __esteh__estehvm__estehvm_h
#define __esteh__estehvm__estehvm_h

#include <stdint.h>
#include <stdlib.h>

typedef struct esteh_vm_init_ {
	int argc;
	char **argv;
	char **envp;
	char *map;
	size_t map_size;
} esteh_vm_init;

typedef struct esteh_vm_rfile {
	char *file_name;
	esteh_vm_init *vm_init;
} esteh_vm_rfile;

uint8_t esteh_vm_run(esteh_vm_init vm_init);
uint8_t esteh_vm_run_file(esteh_vm_rfile vm_run_file);

#endif
