
#ifndef ESTEH_ESTEHVM_HPP
#define ESTEH_ESTEHVM_HPP

#include <cstdio>
#include <unistd.h>
#include <stdint.h>
#include <esteh/esteh_opt.hpp>
#include <esteh/vm/opcode.hpp>

class esteh_vm
{
public:
	esteh_vm(char *, int, esteh_opt **ss);
	void run();
	void parse_file();
	bool linter();
private:
	void execute_opcodes();
	
	int opt_count;
	char *filename;
	char *error_parse = nullptr;
	esteh_opt **opts;
	uint32_t opcode_count;
	esteh_opcode **opcodes;
};

#endif
