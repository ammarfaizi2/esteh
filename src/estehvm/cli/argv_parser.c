
#include <estehvm/cli/argv_parser.h>

void show_help(char *appname) {
	printf("Usage: %s [option] <file> [args...]\n", appname);
	printf("\n");
	printf("  -v\t\tShow version number\n");
}

bool argv_parser(char **argv, int argc, char **error_message, char **file_name, int *app_argc, char ***app_argv) {

	bool got_file_name = false;
	int j = 0;

	for (int i = 1; i < argc; i++) {
		
		size_t len = strlen(argv[i]);

		if (got_file_name) {
			goto get_app_argv;
		}

		if (argv[i][0] == '-') {
			if (len == 1) goto get_file_name;
		}

		continue;

	get_file_name:
		*file_name = (char *)malloc(len + 1);
		memcpy(*file_name, argv[i], len);
		(*file_name)[len] = 0;
		got_file_name = true;

		*app_argc = argc - i;
		*app_argv = (char **)malloc(sizeof(char **) * (*app_argc));
		(*app_argv)[j] = *file_name;
		j++;
		continue;

	get_app_argv:
		(*app_argv)[j] = (char *)malloc(len + 1);
		memcpy((*app_argv)[j], argv[i], len);
		(*app_argv)[j][len] = 0;
		j++;
		continue;

	}


	return true;
}
