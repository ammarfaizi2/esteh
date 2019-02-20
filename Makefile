
COMPILER = g++
LINKER = g++

WD=$(shell pwd)
TARGET_BIN = hako
INC = -I${WD}/include
SRC = ${WD}/src

COMPILER_FLAGS = -Wall -c -std=c++11 ${INC} -fpic
LINKER_FLAGS = 

# Debug Mode
COMPILER_FLAGS += -g -o

# Release Mode
#COMPILER_FLAGS += -O2 -o

RM = rm -vf
CP = cp -vf
LN = ln -vsf
MKDIR = mkdir -vp

SOURCES = $(shell find ${SRC} -name '*.cpp')
OBJECTS = $(SOURCES:%.cpp=%.o)

all: ${OBJECTS} ${TARGET_BIN}

${TARGET_BIN}: ${OBJECTS}
		${LINKER} ${LINKER_FLAGS} -o ${TARGET_BIN} ${OBJECTS}

${OBJECTS}:
		${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

clean:
	${RM} ${OBJECTS} ${TARGET_BIN}
