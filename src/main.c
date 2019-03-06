
#include <stdlib.h>
#include <esteh/usage.h>
#include <esteh/vm/esteh_vm.h>
#include <esteh/argv_parser/argv_parser.h>

int main(int argc, char *argv[])
{

	if (argc == 1) {
		esteh_usage(argv[0]);
		exit(0);
	}

	int app_argc = 0;
	char **app_argv;
	char *filename;
	argv_opt **opts;
	uint16_t opt_count = 0;

	{
		opt_count = esteh_argv_parser(
			argc,
			argv,
			&opts,
			&filename,
			&app_argv,
			&app_argc
		);
	}

	#if 0
		printf("  Filename: %s\n", filename);
		printf("  Opt Count: %d\n", opt_count);
		printf("   App $argc: %d\n", app_argc);
		for (int i = 0; i < app_argc; ++i) {
			printf("   App $argv: %s\n", app_argv[i]);
		}
	#endif

	int exit_code = esteh_vm(filename, opt_count, opts, app_argc, app_argv);
	// esteh_vm_end_shutdown();
	return exit_code;
}
