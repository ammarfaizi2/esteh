
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @licese MIT
 * @version 0.0.1
 *
 * Hako Virtual Machine.
 */

#include <hako/usage.hpp>
#include <hako/hako_opt.hpp>
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

	hako_opt **opts = (hako_opt**)malloc(sizeof(hako_opt *));
	char *filename;

	argv_parser *st = new argv_parser;
	int opt_count = st->run(argc, argv, &filename, opts);
	free(st);
	st = nullptr;

	// debug opts
	#if 1
		for (int i = 0; i < opt_count; ++i)
		{
			printf("opt[%d]:\n", i);
			printf("  need_param\t= %d\n", opts[i]->need_param);
			printf("  opt_name\t= %s\n", opts[i]->opt_name);
			printf("  param\t\t= %s\n", opts[i]->param);	
		}
		printf("\n");

		// print filename
		printf("Filename: %s\n", filename);

		printf("Ok\n");
	#endif
}
