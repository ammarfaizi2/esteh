
#ifndef ESTEH_TOKEN_HPP
#define ESTEH_TOKEN_HPP

#include <stdint.h>
#include <stdlib.h>
#include <esteh/vm/esteh_types.hpp>

typedef union _teavalue_value {
	long long lval;  // For booleans and integers.
	double dval;	 // For floating point numbers.
	struct {		 // For strings.
		char *val;
		uint32_t len;
	} str;
} teavalue_value;

typedef struct _teaval {
	teavalue_value value;
	uint8_t type;
} teaval;

typedef struct _esteh_token {
	uint16_t token;
	uint32_t lineno;
	teaval val;
} esteh_token;

typedef enum _openum {
	opcode_1,
	opcode_2,
	static_value,
	// variable // TODO: Next time.
} openum;

typedef struct _esteh_opcode esteh_opcode;

typedef union _teavald {
	esteh_opcode *opcode_1;
	esteh_opcode *opcode_2;
	teaval static_value;

	// teavar variable; // TODO: Next time.
} teavald;

struct _esteh_opcode {
	void *handler;			// This is a function pointer that will be invoked to perform the operation of the given opcode.
	uint16_t code;			// The opcode being executed.
	uint32_t lineno;		// The line number in the source code the opcode corresponds to.
	teavald op1;
	teavald op2;
	teavald result;
	openum op1_type;
	openum op2_type;
	openum result_type;
};


#define T_UNKNOWN		0
#define T_NOT_A_DATA	0

#define TD_PRINT	20

#define TE_STRING	30
#define TE_INT		31

#define TF_ADD		40
#define TF_MIN		41
#define TF_MUL		42
#define TF_DIV		43

#endif
