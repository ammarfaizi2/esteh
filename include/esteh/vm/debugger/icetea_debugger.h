
#if defined(ESTEH_DEBUG)
#ifndef ESTEH_VM_DEBUGGER_ICETEA_DEBUGGER_H
#define ESTEH_VM_DEBUGGER_ICETEA_DEBUGGER_H

#include <stdint.h>
#include <esteh/macros.h>
#include <esteh/vm/esteh_token.h>

#define TOKEN_DUMPER(TKN) \
	printf(" tkn_code\t: %d\n", TKN->tkn_code); \
	printf(" tkn_type\t: %d\n", TKN->tkn_type); \


#define TEA_VAL_DUMPER(DT) \
	if (DT->type == tea_null) { \
		printf(" Value\t: (null)\n"); \
	} else if (DT->type == t_integer) { \
		printf(" Value\t: %ld\n", DT->val->llval); \
	}

char *get_token_type(uint8_t tkn_type);
char *get_token_name(uint16_t tkn_code);

#endif
#endif
