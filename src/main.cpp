
/**
 * The EsTeh Interpreter
 *
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @version 0.0.1
 * @license MIT
 */

#include <cstdio>
#include <stdlib.h>
#include <esteh/usage.hpp>
#include <esteh/vm/esteh_vm.hpp>
#include <esteh/argv_parser/opt_struct.hpp>
#include <esteh/argv_parser/argv_parser.hpp>

int main(int argc, char *argv[])
{
	
	if (argc == 1) {
		
		// Show usage.
		esteh_usage(argv[0]);

		exit(0);
	}

	char *filename;
	opt_struct **opts;

	{
		argv_parser *st = new argv_parser(argc, argv, &opts);
		st->run(&filename);

		if (st->is_error()) {
			printf("Error: %s\n", st->get_error());
			delete st;
			st = nullptr;
			exit(1);
		}

		delete st;
		st = nullptr;
	}

	{
		esteh_vm *st = new esteh_vm(filename, opts);
	}

	return 0;
}
