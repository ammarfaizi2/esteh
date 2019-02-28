
#include <cstdio>
#include <string>
#include <string.h>
#include <iostream>
#include <esteh/vm/token.hpp>

#define ECX(A,B) case A: return B
#define SRR(S,J,O) \
	for (int i = 0; i < J; ++i) { \
		std::cout << S; \
	} \
	if (O) { \
		std::cout << "|"; \
	}


void node_dumper(tea_node *node, int init) {

	if (init == 1) {
		printf("================================\n");
	}
	if (strcmp(get_token_name(node->token), "unknown")) {
		SRR(" ", init, 1);
		std::cout << "Token: " << std::string(get_token_name(node->token)) << std::endl;
	}
	SRR(" ", init, 1);
	std::cout << "Op1_t: ";
	switch (node->op1_type) {
		case an_unknown:
			printf("an_unknown\n");
		break;
		case a_constant:
			printf("a_constant\n");

			SRR(" ", init, 1);
			std::cout << "ValTp: " << get_value_type(node->op1.constant.data_type) << std::endl;

			SRR(" ", init, 1);

			if (node->op1.constant.data_type == ESTEH_INT) {
				std::cout << "Value: " << std::to_string(node->op1.constant.val.lval) << std::endl;
			} else {
				std::cout << "Value: " << node->op1.constant.val.str.val << std::endl;
			}

		break;
		case a_tea_node:
			printf("a_tea_node\n");
			SRR(" ", init, 1);
			printf("## Tea Node Op1:\n");
			node_dumper(node->op1.node, init + 2);
		break;
	}

	SRR(" ", init, 1);
	std::cout << "Op2_t: ";
	switch (node->op2_type) {
		case an_unknown:
			printf("an_unknown\n");
		break;
		case a_constant:
			printf("a_constant\n");
				
			SRR(" ", init, 1);
			std::cout << "ValTp: " << get_value_type(node->op2.constant.data_type) << std::endl;

			SRR(" ", init, 1);

			if (node->op1.constant.data_type == ESTEH_INT) {
				std::cout << "Value: " << std::to_string(node->op2.constant.val.lval) << std::endl;
			} else {
				std::cout << "Value: " << node->op2.constant.val.str.val << std::endl;
			}

		break;
		case a_tea_node:
			printf("a_tea_node\n");
			node_dumper(node->op2.node, init + 2);
		break;
	}

	SRR(" ", init, 1);
	std::cout << "Rsl_t: ";
	switch (node->result_type) {
		case an_unknown:
			printf("an_unknown\n");
		break;
		case a_constant:
			printf("a_constant\n");
			SRR(" ", init, 1);
			std::cout << "ValTp: " << get_value_type(node->result.constant.data_type) << std::endl;

			SRR(" ", init, 1);
			if (node->result.constant.data_type == ESTEH_INT) {
				std::cout << "Value: " << std::to_string(node->result.constant.val.lval) << std::endl;
			} else {
				std::cout << "Value: " << node->result.constant.val.str.val << std::endl;
			}

		break;
		case a_tea_node:
			printf("a_tea_node\n");
			node_dumper(node->result.node, init + 2);
		break;
	}

	if (init == 1) {
		printf("================================\n");
	}
}

const char *get_value_type(uint16_t type) {
	switch (type) {
		ECX(ESTEH_NULL, "null");
		ECX(ESTEH_STRING, "string");
		ECX(ESTEH_INT, "int");
	}

	return "unknown";
}

const char *get_token_name(uint16_t tkn) {
	switch (tkn) {
		ECX(TA_ADD, "TA_ADD");
		ECX(TA_MIN, "TA_MIN");
		ECX(TA_MUL, "TA_MUL");
		ECX(TA_DIV, "TA_DIV");
		ECX(T_INT, "T_INT");
		ECX(T_STRING, "T_STRING");
		ECX(T_PRINT, "T_PRINT");
		ECX(T_SEMICOLON, "T_SEMICOLON");
	}

	return "unknown";
}

const char *get_token_type(esteh_token *tknd) {
	switch (tknd->type) {
		ECX(t_symbol, "t_symbol");;
		ECX(t_keyword, "t_keyword");;
		ECX(t_operator, "t_operator");;
		ECX(t_constant, "t_constant");;
		ECX(t_end_of_file, "t_end_of_file");;
		default:
			return "unknown";
		break;
	}
}
