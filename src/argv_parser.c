
#include <esteh/esteh.h>
#include <esteh/argv_parser.h>
#include <esteh/estehvm/estehvm.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void usage(char *app_name)
{
	printf("Usage: %s [options] <file>\n", app_name);
}

bool esteh_argv_parser(
	uint8_t *exit_code,
	esteh_vm_rfile *vm_run_file,
	int argc,
	char **argv,
	char **envp
)
{
	if (argc == 1) {
		usage(argv[0]);
		*exit_code = 0;
		return false;
	}

	int i;
	size_t len;

	for (i = 1; i < argc; i++) {
		len = strlen(argv[i]);
		if (len > 0) {
			if (argv[i][0] == '-')	{
				if (len == 1) {
					fprintf(stderr, "Invalid option \"-\" at offset %d\n", i);
					return false;
				}

				if (argv[i][1] == '-') {
					
				} else {
					if (len == 2) {
						switch (argv[i][1]) {
							case 'v':
								printf("%s\n", ESTEH_VERSION);
								*exit_code = 0;
								return false;
							break;

							default:
								fprintf(stderr, "Invalid option \"-%c\" at offset %d\n", argv[i][1], i);
								return false;
							break;
						}
					}
				}

			} else {
				vm_run_file->file_name = argv[i];
				vm_run_file->vm_init = (esteh_vm_init *)malloc(sizeof(esteh_vm_init));
				vm_run_file->vm_init->argc = argc - i;
				vm_run_file->vm_init->argv = &(argv[i]);
				vm_run_file->vm_init->envp = envp;
				return true;
			}
		}
	}

	fprintf(stderr, "Missing file on arguments!\n");

	return false;
}
