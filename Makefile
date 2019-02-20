
COMPILER = g++
LINKER = g++
DEBUG_MODE = 1

ROOT_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
TARGET_BIN = ${ROOT_DIR}/hako
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
OBJECTS = $(SOURCES:%.cpp=%.o)

all: ${OBJECTS} ${TARGET_BIN}

${TARGET_BIN}: ${OBJECTS}
		${LINKER} ${LINKER_FLAGS} -o ${TARGET_BIN} ${OBJECTS}

${OBJECTS}:
		${COMPILER} ${COMPILER_FLAGS} ${@:%.o=%.cpp} -o $@

strip:
	${STRIP}

clean:
	${RM} ${OBJECTS} ${TARGET_BIN}
