
#ifndef ESTEH__VM__ESTEHVM_H
#define ESTEH__VM__ESTEHVM_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int estehvm();
void vm_openfile();
void vm_token_clean_up();
void vm_lexical();

#endif
