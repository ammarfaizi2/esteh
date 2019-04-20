
#include <esteh/usage.h>
#include <esteh/vm/estehvm.h>
#include <esteh/vm/buffer/buffer.h>
#include <esteh/argv_parser/parser.h>

int app_argc = 1;
char **app_argv;
char *filename = NULL;

int main(int argc, char *argv[])
{	
	if (argc == 1) {
		usage(argv[0]);
		exit(0);
	}

	argv_parser(argc, argv);

	if (filename == NULL) {
		printf("Missing filename!\n");
		exit(1);
	}

	exit(estehvm());

	return 0;
}
