
#ifndef HAKO_ARGV_PARSER_HPP
#define HAKO_ARGV_PARSER_HPP

#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <hako/hako_opt.hpp>

class argv_parser
{
public:
	argv_parser();
	int run(int, char **, char **, hako_opt **);

private:
	void a1_opts(int, hako_opt *, char *, int);
	void a2_opts(int, hako_opt *, char *);

	int argc;
	int skip = 0;
	int opt_count = 0;
	char **argv_ptr;
	hako_opt **opt_ptr;
};

#endif

