
#include <hako/vm/hakovm.hpp>
#include <hako/vm/code_parser.hpp>

code_parser::code_parser() {

}

void code_parser::buf_read(char *buf_code, size_t read_bytes) {
	this->buf_code = buf_code;
	this->read_bytes = read_bytes;
}

void code_parser::build_opcode() {
	for (size_t i = 0; i < this->read_bytes; ++i) {
		printf("%c\n", this->buf_code[i]);
	}
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
