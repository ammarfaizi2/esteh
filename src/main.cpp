
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @licese MIT
 * @version 0.0.1
 *
 * Hako Virtual Machine.
 */

#include <hako/usage.hpp>
#include <hako/argv_parser.hpp>

int main(int argc, char *argv[])
{
	if (argc == 1) {
		usage *st = new usage(argv[0]);
		st->general();
		free(st);
		st = nullptr;
		exit(0);
	}

	printf("%s\n", argv[1]);

	char **options = (char**)malloc(sizeof(char*) * (argc - 1));
	char *filename;

	argv_parser *st = new argv_parser;
	st->run(argc, &argv, &filename, &options);
	free(st);
	st = nullptr;
	printf("Ok\n");
}
