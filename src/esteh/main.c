
#include <esteh/usage.h>
#include <esteh/argv_parser/parser.h>

int app_argc = 1;
char **app_argv;

int main(int argc, char *argv[])
{	
	if (argc == 1) {
		usage(argv[0]);
		exit(0);
	}

	argv_parser(argc, argv);

	return 0;
}
