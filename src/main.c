
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <esteh/usage.h>
#include <esteh/argv_parser/argv_parser.h>

int main(int argc, char *argv[])
{
	
	if (argc == 1) {
		esteh_usage(argv[0]);
		exit(0);
	}

	char *filename;
	argv_opt **opts = (argv_opt **)malloc(sizeof(argv_opt *));

	{
		bool is_error = false;
		char *error_msg = (char *)malloc(sizeof(char));
		esteh_argv_parser(argv, &opts, &is_error, &error_msg, &filename);
		free(error_msg);
		error_msg = NULL;
	}


	return 0;
}
