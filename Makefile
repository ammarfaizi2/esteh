
COMPILER = g++
LINKER = g++

COMPILER_FLAGS = -Wall -c -g -std=c++11 -fpic -o
LINKER_FLAGS = -shared

RM = rm -vf
CP = cp -vf
LN = ln -vsf
MKDIR = mkdir -vp

#SOURCES = $(wildcard src/*.cpp)
SOURCES = $(shell find src/ -name '*.cpp')
OBJECTS = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${EXTENSION}

	${EXTENSION}: ${OBJECTS}
		${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

	${OBJECTS}:
		${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

clean:
	${RM} ${OBJECTS} ${EXTENSION}