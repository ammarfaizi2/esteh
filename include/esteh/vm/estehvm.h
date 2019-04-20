
#ifndef ESTEH__VM__ESTEHVM_H
#define ESTEH__VM__ESTEHVM_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int estehvm();
void *esteh_vm_openfile(char *filename, size_t *filesize, int *filefd);

int esteh_vm_init();
int esteh_vm_shutdown();
int esteh_vm_execute();



#endif
