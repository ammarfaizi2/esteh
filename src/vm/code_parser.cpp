
#include <hako/vm/token.hpp>
#include <hako/vm/hakovm.hpp>
#include <hako/vm/code_parser.hpp>

code_parser::code_parser() {

}

void code_parser::buf_read(char *buf_code, size_t read_bytes) {
	this->buf_code = buf_code;
	this->read_bytes = read_bytes;
}

void code_parser::finish() {

}

int code_parser::is_ok() {
	return 1;
}

char *code_parser::get_error() {
	return nullptr;
}

size_t code_parser::get_error_length() {
	return 0;
}

int code_parser::token_d(char *token) {

	if (!strcmp(token, "echo")) {
		return TD_ECHO;
	}


	printf("Error: Unknown token \"%s\"\n", token);
	return T_UNKNOWN;
}

void code_parser::build_opcode() {

	#define $rb this->buf_code[i]

	hako_opcode **opcodes = (hako_opcode **)malloc(sizeof(hako_opcode *));
	size_t opcodes_size = 0;

	int 
		in_quoto = 0,
		token_d = 0,
		line = 1;

	char *cur_token = (char *)malloc(sizeof(char));
	size_t cur_token_size = 0;

	for (size_t i = 0; i < this->read_bytes; ++i) {

		if ($rb == 10) {
			line++;
		}

		if ($rb == 34) {
			if (in_quoto) {
				cur_token = (char*)realloc(cur_token, ++cur_token_size);
				cur_token[cur_token_size - 1] = '\0';
				
				if (opcodes_size > 0) {
					opcodes = (hako_opcode **)realloc(opcodes, sizeof(hako_opcode *) * opcodes_size);
				}

				opcodes[opcodes_size] = (hako_opcode *)malloc(sizeof(hako_opcode));
				opcodes[opcodes_size]->code = TE_STRING;
				opcodes[opcodes_size]->line = line;
				opcodes[opcodes_size]->content = (char *)malloc(sizeof(char) * cur_token_size);
				memcpy(opcodes[opcodes_size++]->content, cur_token, sizeof(char) * cur_token_size);

				in_quoto = 0;
			} else {
				in_quoto = 1;
			}
			continue;
		} else {
			if (in_quoto) {
				cur_token = (char *)realloc(cur_token, cur_token_size + 1);
				cur_token[cur_token_size++] = $rb;
				continue;
			}
		}

		if (
			($rb >= 65 && $rb <= 90) ||
			($rb >= 97 && $rb <= 122) ||
			($rb == 95)
		) {
			if (token_d) {
				cur_token = (char *)realloc(cur_token, cur_token_size);
			} else {
				token_d = 1;
			}
			cur_token[cur_token_size++] = $rb;
			continue;
		}
		
		if (token_d) {
			cur_token = (char*)realloc(cur_token, ++cur_token_size);
			cur_token[cur_token_size - 1] = '\0';
			
			if (opcodes_size > 0) {
				opcodes = (hako_opcode **)realloc(opcodes, sizeof(hako_opcode *) * opcodes_size);
			}

			opcodes[opcodes_size] = (hako_opcode *)malloc(sizeof(hako_opcode));
			opcodes[opcodes_size]->code = this->token_d(cur_token);
			opcodes[opcodes_size]->line = line;
			opcodes[opcodes_size++]->content = nullptr;

			token_d = 0;
			cur_token_size = 0;
		}
	}

	int skip = 0;

	for (size_t i = 0; i < opcodes_size; ++i) {

		if (skip) {
			skip = 0;
			continue;
		}

		switch (opcodes[i]->code) {
			case TD_ECHO:
				printf("%s\n", opcodes[i+1]->content);
				skip = 1;
			break;
		}
	}

}
