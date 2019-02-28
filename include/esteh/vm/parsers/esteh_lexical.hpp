
#ifndef ESTEH_VM_PARSERS_ESTEH_LEXICAL_HPP
#define ESTEH_VM_PARSERS_ESTEH_LEXICAL_HPP

#include <stdint.h>
#include <esteh/vm/esteh_token.hpp>

class esteh_lexical
{
public:
	esteh_lexical(char *filename, esteh_token ***tokens);
	void run();
private:
	void parse();
	void open_file();
	void analyze_token();
	char escape_char(char c);
	void build_node(esteh_token *tkn);
	uint16_t tokenize(char *token, uint32_t line, esteh_token **tkn);
	void recursive_opeator_scan(
		tea_node **node,
		esteh_token *op1,
		esteh_token *op2,
		bool in_recursive = false
	);

	int file_des;
	char *map;
	char *filename;
	uint32_t anal_ptr = 0;
	uint32_t filesize = 0;
	uint32_t node_count = 0;
	uint32_t token_count = 0;
	uint32_t cur_node_alloc = TEA_NODE_FIRST_ALLOC;	
	esteh_token ***tokens;
	tea_node **nodes;
};

#endif
