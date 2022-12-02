#TODO: Set the path of your llvm-project directory
LLVM_DIR=/Users/eshaj/Documents/repo/llvm-project

CLANG_DIR=$(LLVM_DIR)/clang
CLANG_TOOL_DIR=$(LLVM_DIR)/build/tools/clang

SOURCE_DIR=src
OBJS=main.o instrumentor.o node_visitor.o invalid_file_exception.o

CXX_FLAGS += -I$(LLVM_DIR)/llvm/include 
CXX_FLAGS += -I$(LLVM_DIR)/build/include
CXX_FLAGS += -I$(CLANG_DIR)/include 
CXX_FLAGS += -I$(CLANG_TOOL_DIR)/include -std=c++17 -fno-rtti
CXX_FLAGS += -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS
CXX_FLAGS += -D__STDC_LIMIT_MACROS 
CXX_FLAGS += -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1
#CXX_FLAGS += `$(LLVM_DIR)/build/bin/llvm-config --cxxflags`

LDFLAGS = `$(LLVM_DIR)/build/bin/llvm-config --ldflags`
LDFLAGS += -lclangTooling
LDFLAGS += -lclangFrontendTool
LDFLAGS += -lclangFrontend
LDFLAGS += -lclangDriver
LDFLAGS += -lclangSerialization
LDFLAGS += -lclangCodeGen
LDFLAGS += -lclangParse
LDFLAGS += -lclangSema
LDFLAGS += -lclangStaticAnalyzerFrontend
LDFLAGS += -lclangStaticAnalyzerCheckers
LDFLAGS += -lclangStaticAnalyzerCore
LDFLAGS += -lclangAnalysis
LDFLAGS += -lclangARCMigrate
LDFLAGS += -lclangRewrite
LDFLAGS += -lclangRewriteFrontend
LDFLAGS += -lclangEdit
LDFLAGS += -lclangAST
LDFLAGS += -lclangLex
LDFLAGS += -lclangBasic
LDFLAGS += -lclangSupport
LDFLAGS += -lclang
LDFLAGS += `$(LLVM_DIR)/build/bin/llvm-config --libs`
LDFLAGS += `$(LLVM_DIR)/build/bin/llvm-config --system-libs`

# lzstd library is additionally needed for MacOS
ifeq ($(shell uname -o), Darwin)
	LDFLAGS += -L$(shell brew --prefix)/lib
endif

SYS=$(shell uname -s)
ifeq ($(SYS), Darwin)
	LIB_ENV_VAR = DYLD_LIBRARY_PATH
else ifeq ($(SYS), Linux)
	LIB_ENV_VAR = LD_LIBRARY_PATH
endif

all: instrument example/run_example.sh

instrument: $(OBJS)
	clang++ -g -Wall -o $@ $^ $(LDFLAGS)

%.o: $(SOURCE_DIR)/%.cc
	clang++ $(CXX_FLAGS) -c $< -o $@

example/run_example.sh:
	@echo "#!/bin/bash" > example/run_example.sh	
	@echo "cd .. && make && $(LIB_ENV_VAR)=$(LLVM_DIR)/build/lib ./instrument example/foo.c" >> example/run_example.sh	
	@chmod 755 example/run_example.sh

clean:
	rm -rf *.o instrument

.PHONY: all clean
