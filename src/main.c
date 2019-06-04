
#include <estehvm/vm/esteh.h>
#include <estehvm/cli/argv_parser.h>
#include <estehvm/vm/stdio.h>

/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @license MIT
 * @version 0.0.1
 */

int main(int argc, char **argv, char **envp)
{
	if (argc == 1) {
		show_help(argv[0]);
		exit(0);
	}

	char *error_msg = NULL;
	char *file_name = NULL;
	int app_argc = 0;
	char **app_argv = NULL;

	if (!argv_parser(argv, argc, &error_msg, &file_name, &app_argc, &app_argv)) {

		if (file_name != NULL) {
			free(file_name);
			file_name = NULL;
		}

		if (app_argv != NULL) {
			for (int i = 0; i < app_argc; ++i) {
				free(app_argv[i]);
				app_argv[i] = NULL;
			}
			free(app_argv);
			app_argv = NULL;
		}

		printf("%s\n", error_msg);
		free(error_msg);
		exit(1);
	}

	uint8_t exit_code = esteh_vm_run_file(file_name, app_argc, app_argv);


	exit(exit_code);

	return 0;
}
