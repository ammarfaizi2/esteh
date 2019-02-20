
COMPILER = g++
LINKER = g++

COMPILER_FLAGS = -Wall -c -g -std=c++11 -fpic -o
LINKER_FLAGS = 

BIN = hako

RM = rm -vf
CP = cp -vf
LN = ln -vsf
MKDIR = mkdir -vp

#SOURCES = $(wildcard src/*.cpp)
SOURCES = $(shell find src/ -name '*.cpp')
OBJECTS = $(SOURCES:%.cpp=%.o)

all: ${OBJECTS} ${BIN}

${BIN}: ${OBJECTS}
		${LINKER} ${LINKER_FLAGS} -o ${BIN} ${OBJECTS}

${OBJECTS}:
		${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

clean:
	${RM} ${OBJECTS} ${BIN}