
#include <cstdio>
#include <esteh/vm/opcode.hpp>
#include <esteh/vm/code_parser.hpp>

#define _OPCODES_ALLOC sizeof(esteh_opcode *) * 10;
#define _OPCODES_REALLOC \
	if ( (((*opcode_count) + 2) * sizeof(esteh_opcode *)) >= *cur_opcodes_alloc ) { \
		*cur_opcodes_alloc = (((*opcode_count) + 2) * sizeof(esteh_opcode *)) + _OPCODES_ALLOC \
		printf("%d\n", *cur_opcodes_alloc); \
		(**opcodes) = (esteh_opcode **)realloc(**opcodes, *cur_opcodes_alloc); \
	} \

void code_parser::build_opcode(
	uint32_t opcode_offset,
	uint32_t tokens_count,
	uint32_t *opcode_count,
	uint32_t *cur_opcodes_alloc,
	bool opcode_must_has_operand,
	esteh_token **tokens,
	esteh_opcode ****opcodes
) {
	switch (tokens[opcode_offset]->token) {
		case TD_PRINT:
			(**opcodes)[*opcode_count] = (esteh_opcode *)malloc(sizeof(esteh_opcode));
			(**opcodes)[*opcode_count]->code = TD_PRINT;
			(**opcodes)[*opcode_count]->lineno = tokens[opcode_offset]->lineno;
			uint32_t i = opcode_offset + 1;
			while (i < tokens_count) {
				switch (tokens[i]->token) {
					case TE_STRING:
					case TE_INT:

						_OPCODES_REALLOC

						(**opcodes)[*opcode_count]->op1.static_value.value = tokens[i]->val.value;
						(**opcodes)[*opcode_count]->op1.static_value.type = tokens[i]->val.type;
						(**opcodes)[*opcode_count]->op1_type = static_value;
						(*opcode_count)++;
					break;
				}
				i++;
			}
		break;
	}

}
