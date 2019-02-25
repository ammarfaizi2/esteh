
#ifndef ESTEH_TOKEN_HPP
#define ESTEH_TOKEN_HPP

#include <stdint.h>
#include <stdlib.h>

typedef union _teavalue_value {
	long long lval;  // For booleans and integers.
	double dval;	 // For floating point numbers.
	struct {		 // For strings
		char *val;
		size_t len;
	} str;
} teavalue_value;

typedef struct _teaval {
	teavalue_value value;
	uint8_t type;
} teaval;

typedef struct _esteh_opcode {
	void *handler;		// This is a function pointer that will be invoked to perform the operation of the given opcode.
	uint16_t code;		// The opcode being executed.
	uint32_t lineno;	// The line number in the source code the opcode corresponds to.
	teaval op1;
	teaval op2;
	teaval result;
	void *content;
} esteh_opcode;


#define ESTEH_TYPE_NULL 	0b0001
#define ESTEH_TYPE_STRING	0b0010
#define ESTEH_TYPE_INT		0b0100

#define T_UNKNOWN 0

#define TC_NOTHING 100

#define TD_PRINT 200

#define TE_STRING 300
#define TE_INT 301

#define TF_ADD 1 // +
#define TF_MIN 2 // -
#define TF_MUL 4 // *
#define TF_DIV 8 // /

#endif
