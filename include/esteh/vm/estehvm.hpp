
#ifndef ESTEH_ESTEHVM_HPP
#define ESTEH_ESTEHVM_HPP

#include <cstdio>
#include <unistd.h>
#include <esteh/esteh_opt.hpp>

class estehvm
{
public:
	estehvm(char *, int, esteh_opt **ss);
	void run();
	void init_file_streamer();
	void parse_file(int);
	int linter(char **);
private:
	int opt_count;
	char *filename;
	char *error_parse = nullptr;
	esteh_opt **opts;
	FILE *hdf;
};

#endif
