
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

	hako_opcode ** opcodes = (hako_opcode **)malloc(sizeof(hako_opcode *));
	size_t opcodes_size = 0;

	char *token;

	// Initialize token with 1 char allocation.
	token = (char *)malloc(sizeof(char));

	size_t token_size = 1;

	for (size_t i = 0; i < this->read_bytes; ++i) {
		
		if ($rb == 10) {
			line++;
		}
		
		if ($rb == '"') {

			if (in_dquo) {
				// End of a string.

			} else {
				// Start of a string.
			}
			continue;
		} else if (in_dquo) {

			continue;
		}


		if (
			// A-Z
			($rb >= 65 && $rb <= 90) ||

			// a-z
			($rb >= 97 && $rb <= 122) ||

			// _
			($rb == 95)
		) {
			if (in_te) {
				if (token_size > 1) {
					token = (char *)realloc(token, token_size);
				}
			} else {

				// We don't need realloc here, because it has been allocated with 1 char.
				in_te = 1;
			}
			token[token_size - 1] = $rb;
			token_size++;
			continue;
		} else {
			if (in_te) {
				in_te = 0;
			}
		}
	}

	printf("%s\n", token);
	exit(0);

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
