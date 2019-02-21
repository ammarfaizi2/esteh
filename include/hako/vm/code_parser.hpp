
#ifndef HAKO_CODE_PARSER_HPP
#define HAKO_CODE_PARSER_HPP

#include <string.h>

class code_parser
{
public:
	code_parser();
	~code_parser();
	void buf_read(char *, size_t);
	void build_opcode();
	void finish();
	int is_ok();
	size_t get_error_length();
	char *get_error();
private:
	char *buf_code;
	size_t read_bytes;
};

#endif
