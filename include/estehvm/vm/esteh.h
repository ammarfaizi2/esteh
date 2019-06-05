
#ifndef estehvm__vm__esteh_h
#define estehvm__vm__esteh_h

#include <stdint.h>
#include <estehvm/vm/token.h>
#include <estehvm/vm/bytecode.h>

uint8_t esteh_vm_run();
void esteh_vm_init_io();
void esteh_vm_shutdown_io();
uint8_t esteh_vm_run_file(char *file_name, int app_argc, char **app_argv);
void esteh_vm_token_clean_up(esteh_token ***tokens, uint32_t token_count);
void esteh_vm_tokenizer(esteh_token ***tokens, uint32_t token_count, esteh_bytecode ***bytecodes);

#endif
