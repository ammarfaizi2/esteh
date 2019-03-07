
#if defined(ESTEH_DEBUG)
#ifndef ESTEH_VM_DEBUGGER_ICETEA_DEBUGGER_H
#define ESTEH_VM_DEBUGGER_ICETEA_DEBUGGER_H

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <esteh/macros.h>
#include <esteh/vm/esteh_token.h>

#define TOKEN_DUMPER(TKN) \
	printf("============================\n"); \
	printf(" tkn_type: %s\n", get_token_type(TKN->tkn_type)); \
	printf(" tkn_code: %s\n", get_token_name(TKN->tkn_code)); \
	if (TKN->tkn_type == t_constant) { \
		TEA_VAL_DUMPER(TKN->tkn_val.data) \
	} \
	printf("============================\n"); \


#define TEA_VAL_DUMPER(DT) \
	if (DT.type == tea_null) { \
		printf(" Value\t: (null)\n"); \
	} else if (DT.type == tea_integer) { \
		printf(" Value\t: %lld\n", DT.val.llval); \
	} else if (DT.type == tea_string) { \
		write(1, " Value\t: \"", sizeof(" Value\t: \"") - 1); \
		write(1, DT.val.str.val, DT.val.str.len); \
		write(1, "\"\n", sizeof("\"\n") - 1); \
		write(1, " Length\t: ", sizeof(" Length\t: ") - 1); \
		printf("%ld\n", DT.val.str.len); \
		fflush(stdout); \
	}

char *get_token_type(uint8_t tkn_type);
char *get_token_name(uint16_t tkn_code);

#endif
#endif
