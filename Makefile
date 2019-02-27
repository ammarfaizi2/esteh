
COMPILER = g++
LINKER = g++
DEBUG_MODE = 0

ROOT_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
BIN_NAME = esteh
TARGET_BIN = ${ROOT_DIR}/${BIN_NAME}
INC = -I${ROOT_DIR}/include
SRC = ${ROOT_DIR}/src

_COMPILER_FLAGS = -Wall -c -std=c++11 ${INC} -fpic
LINKER_FLAGS = -Wall

ifneq (${DEBUG_MODE},0)
	COMPILER_FLAGS = ${_COMPILER_FLAGS} -ggdb -g3
else
	COMPILER_FLAGS = ${_COMPILER_FLAGS} -O2 -s
endif

RM = rm -vf
CP = cp -vf
LN = ln -vsf
MKDIR = mkdir -vp

STRIP = strip -s ${TARGET_BIN}

SOURCES = $(shell find ${SRC} -name '*.cpp')
OBJECTS = $(SOURCES:%.cpp=%.cpp.o)

all: ${OBJECTS} ${TARGET_BIN}

${TARGET_BIN}: ${OBJECTS}
	${LINKER} ${LINKER_FLAGS} -o ${TARGET_BIN} ${OBJECTS}

${OBJECTS}:
	${COMPILER} ${COMPILER_FLAGS} ${@:%.o=%} -o $@

install:
	${CP} ${TARGET_BIN} /usr/bin/${BIN_NAME}

uninstall:
	${RM} /usr/bin/${BIN_NAME}

test:
	php ${ROOT_DIR}/run_test.php
	chmod +x /usr/bin/${BIN_NAME}

strip:
	${STRIP}

clean:
	${RM} ${OBJECTS} ${TARGET_BIN}
