
#ifndef ESTEH_VM_PARSERS_ESTEH_LEXICAL_HPP
#define ESTEH_VM_PARSERS_ESTEH_LEXICAL_HPP

#include <stdint.h>

class esteh_lexical
{
public:
	esteh_lexical(char *filename, esteh_token ***tokens);
	void run();
private:
	void open_file();
	void build_node();
	char escape_char(char c);
	uint16_t tokenize(char *token, uint32_t line);

	int file_des;
	char *map;
	char *filename;
	uint32_t filesize = 0;
	uint32_t token_count = 0;
	esteh_token ***tokens;
};

#endif
