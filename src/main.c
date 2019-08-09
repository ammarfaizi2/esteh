
#include <esteh/argv_parser.h>
#include <esteh/estehvm/estehvm.h>

#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[], char *envp[])
{
	uint8_t ret = 1;
	esteh_vm_rfile vm_run_file;

	if (esteh_argv_parser(&ret, &vm_run_file, argc, argv, envp)) {
		ret = esteh_vm_run_file(vm_run_file);
	}

	exit(ret);
}
