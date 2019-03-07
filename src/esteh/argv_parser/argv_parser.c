
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <esteh/macros.h>
#include <esteh/version.h>
#include <esteh/argv_parser/argv_parser.h>

#define $opt (*opts)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

static inline void esteh_version() {
	write(1, ESTEH_VERSION "\n", sizeof(ESTEH_VERSION "\n") - 1);
}

#pragma GCC diagnostic pop

uint16_t esteh_argv_parser(
	int argc,
	char **argv,
	argv_opt ***opts,
	char **filename,
	char ***app_argv,
	int *app_argc
) {
	
	uint16_t count_opt = 0;
	bool got_filename = false;
	size_t current_opts_size = sizeof(argv_opt **) * 3;
	$opt = (argv_opt **)malloc(current_opts_size);
	*app_argv = (char **)malloc(sizeof(char **));

	for (int i = 1; i < argc; ++i) {
		if (got_filename) {
			(*app_argc)++;
			*app_argv = realloc(*app_argv, sizeof(char **) * (*app_argc));
			(*app_argv)[(*app_argc) - 1] = argv[i];
		} else {
			size_t len = strlen(argv[i]);
			if (argv[i][0] == '-') {
				if (len == 1) {
					printf("Invalid option \"-\"\n");
					exit(1);
				}
				if (argv[i][1] != '-') {
					switch(argv[i][1]) {
						case 'v':
							esteh_version();
							exit(0);
						break;
					}
				} else {
					if (len == 2) {
						printf("Invalid option \"--\"\n");
						exit(1);
					}
					argv[i] = argv[i]+2;
					if (SCMP(argv[i], "version")) {
						esteh_version();
						exit(0);
					}
				}
				continue;
			}

			*app_argc = 1;
			*filename = argv[i];
			(*app_argv)[0] = argv[i];
			got_filename = true;
		}
	}

	return count_opt;
}
