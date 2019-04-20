
#ifndef ESTEH__VM__DEBUGGER__TOKEN_DUMPER_H
#define ESTEH__VM__DEBUGGER__TOKEN_DUMPER_H

#include <stdio.h>
#include <stdint.h>
#include <esteh/vm/parser/token.h>

void esteh_token_dumper(tea_token **tokens, uint32_t token_amount);

#endif
