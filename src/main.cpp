
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

	char **options = (char**)malloc(sizeof(char*) * (argc - 1));
	char *filename;

	argv_parser(argc, &argv, &filename, &options);
}
