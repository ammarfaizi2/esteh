
#ifndef ESTEH_ARGV_PARSER_HPP
#define ESTEH_ARGV_PARSER_HPP

#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <esteh/esteh_opt.hpp>

class argv_parser
{
public:
	argv_parser();
	int run(int, char **, char **, esteh_opt **);

private:
	void a1_opts(int, esteh_opt *, char *, int);
	void a2_opts(int, esteh_opt *, char *);

	int argc;
	int skip = 0;
	int opt_count = 0;
	char **argv_ptr;
	esteh_opt **opt_ptr;
};

#endif

