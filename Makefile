
COMPILER = g++
LINKER = g++

TARGET_BIN = hako
INCLUDE_DIR = include

COMPILER_FLAGS = -Wall -c -std=c++11 -I ${INCLUDE_DIR} -fpic
LINKER_FLAGS = 

# Debug Mode
COMPILER_FLAGS += -g -o

# Release Mode
#COMPILER_FLAGS += -O2 -o

RM = rm -vf
CP = cp -vf
LN = ln -vsf
MKDIR = mkdir -vp

SOURCES = $(shell find src/ -name '*.cpp')
OBJECTS = $(SOURCES:%.cpp=%.o)

all: ${OBJECTS} ${BIN}

${BIN}: ${OBJECTS}
		${LINKER} ${LINKER_FLAGS} -o ${BIN} ${OBJECTS}

${OBJECTS}:
		${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

clean:
	${RM} ${OBJECTS} ${BIN}
