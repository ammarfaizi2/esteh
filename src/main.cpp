
#include <stdlib.h>
#include <esteh/usage.hpp>
#include <esteh/argv_parser/opt_struct.hpp>
#include <esteh/argv_parser/argv_parser.hpp>

int main(int argc, char *argv[])
{
	
	if (argc == 1) {
		
		// Show usage.
		esteh_usage(argv[0]);

		exit(0);
	}


	opt_struct **opts;

	{
		argv_parser *st = new argv_parser(argc, argv);
		st->run(&opts);
		delete st;
		st = nullptr;
	}

	return 0;
}
