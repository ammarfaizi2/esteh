
#ifndef ESTEH_CODE_PARSER_HPP
#define ESTEH_CODE_PARSER_HPP

#include <string.h>
#include <stdlib.h>

class code_parser
{
public:
	code_parser();
	~code_parser();
	void build_opcode();
	void finish();
	int is_ok();
	size_t get_error_length();
	char *get_error();
	void add_file_handler(FILE *, char *);
private:
	void buf_read();
	int token_d(char *);
	FILE *hdf;
	char *buf_code;
	char *error_parse;
	size_t read_bytes;
};

#endif
