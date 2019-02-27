
#ifndef ESTEH_ARGV_PARSER_ARGV_PARSER_HPP
#define ESTEH_ARGV_PARSER_ARGV_PARSER_HPP

#include <esteh/argv_parser/opt_struct.hpp>

class argv_parser 
{
public:
	argv_parser(int argc, char **argv);
	void run(opt_struct ***opts);
	bool is_error();
	char *get_error();
private:
	void parse();

	int argc;
	char **argv;
	char *error = nullptr;
};

#endif
