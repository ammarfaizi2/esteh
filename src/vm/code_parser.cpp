
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

	int in_dquo = 0,
		in_te = 0,
		line = 1;

	hako_opcode **opcodes = (hako_opcode **)malloc(sizeof(hako_opcode *));
	size_t opcodes_size = 0;

	char *token;

	// Initialize token with 1 char allocation.
	token = (char *)malloc(sizeof(char));

	size_t token_size = 0;

	for (size_t i = 0; i < this->read_bytes; ++i) {
		if ($rb == 10) {
			line++;
		}

		if ($rb == '"') {
			if (in_dquo) {
				// End of a string.
				opcodes = (hako_opcode **)realloc(opcodes, sizeof(hako_opcode *) * (opcodes_size + 1));
				opcodes[opcodes_size] = (hako_opcode *)malloc(sizeof(hako_opcode));
				opcodes[opcodes_size]->line = line;
				opcodes[opcodes_size]->code = TE_STRING;
				opcodes[opcodes_size]->content = (char *)malloc(sizeof(char) * (token_size + 1));
				memcpy(opcodes[opcodes_size]->content, token, sizeof(char) * (token_size + 1));
				opcodes_size++;
				in_dquo = 0;
				token_size = 0;
			} else {
				// Start of a string.
				in_dquo = 1;
			}
			continue;
		} else if (in_dquo) {
			token = (char *)realloc(token, token_size + 2);
			token[token_size] = $rb;
			token[token_size + 1] = '\0';
			token_size++;
			continue;
		}

		if (
			($rb >= 65 && $rb <= 90) ||
			($rb >= 97 && $rb <= 122) ||
			($rb == 95)
		) {
			if (!in_te) {
				in_te = 1;
			}
			token = (char *)realloc(token, token_size + 2);
			token[token_size] = $rb;
			token[token_size + 1] = '\0';
			token_size++;
		} else if (in_te) {
			opcodes = (hako_opcode **)realloc(opcodes, sizeof(hako_opcode *) * (opcodes_size + 1));
			opcodes[opcodes_size] = (hako_opcode *)malloc(sizeof(hako_opcode));
			opcodes[opcodes_size]->line = line;
			opcodes[opcodes_size]->code = this->token_d(token);
			opcodes[opcodes_size]->content = nullptr;
			opcodes_size++;
			in_te = 0;
			token_size = 0;
		}
	}

	free(token);
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
