
#ifndef estehvm__vm__esteh_h
#define estehvm__vm__esteh_h

#include <stdint.h>
#include <estehvm/vm/token.h>

uint8_t esteh_vm_run();
uint8_t esteh_vm_run_file(char *file_name, int app_argc, char **app_argv);
void esteh_token_clean_up(esteh_token ***tokens, uint32_t token_count);

#endif
