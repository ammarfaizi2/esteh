
#ifndef ESTEH_VM_ESTEH_VM_HPP
#define ESTEH_VM_ESTEH_VM_HPP

#include <esteh/vm/esteh_token.hpp>
#include <esteh/argv_parser/opt_struct.hpp>

class esteh_vm
{
public:
	esteh_vm(char *filename, opt_struct **opts);
	void run();
private:
	void open_file();
	void lexical_analyzer();
	char *filename;
	opt_struct **opts;
	esteh_token **tokens;
};

#endif
