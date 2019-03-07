
COMPILER = gcc
LINKER = gcc
DEBUG_MODE = 1

ROOT_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
BIN_NAME = esteh
TARGET_BIN = ${ROOT_DIR}/${BIN_NAME}
INC = -I${ROOT_DIR}/include
SRC = ${ROOT_DIR}/src

_COMPILER_FLAGS = -Wall -c ${INC} -fpic
LINKER_FLAGS = -Wall

ifneq (${DEBUG_MODE},0)
	COMPILER_FLAGS = ${_COMPILER_FLAGS} -ggdb -g3 -D ESTEH_DEBUG
else
	COMPILER_FLAGS = ${_COMPILER_FLAGS} -O2 -s
endif

RM = rm -vf
CP = cp -vf
LN = ln -vsf
MKDIR = mkdir -vp

STRIP = strip -s ${TARGET_BIN}

SOURCES = $(shell find ${SRC} -name '*.c')
OBJECTS = $(SOURCES:%.c=%.c.o)

all: ${OBJECTS} ${TARGET_BIN}

${TARGET_BIN}: ${OBJECTS}
	${LINKER} ${LINKER_FLAGS} -o ${TARGET_BIN} ${OBJECTS}

${OBJECTS}:
	${COMPILER} ${COMPILER_FLAGS} ${@:%.o=%} -o $@

install:
	${CP} ${TARGET_BIN} /usr/bin/${BIN_NAME}
	chmod +x /usr/bin/${BIN_NAME}

uninstall:
	${RM} /usr/bin/${BIN_NAME}

test:
	php ${ROOT_DIR}/run_test.php

strip:
	${STRIP}

clean:
	${RM} ${OBJECTS} ${TARGET_BIN}
