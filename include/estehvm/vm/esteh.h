
#ifndef estehvm__vm__esteh_h
#define estehvm__vm__esteh_h

#include <stdint.h>

uint8_t esteh_vm_run();
uint8_t esteh_vm_run_file(char *file_name, int app_argc, char **app_argv);

#endif
