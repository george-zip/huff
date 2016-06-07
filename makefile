HUFF_TARGET=bin/huff
LIB_TARGET=lib/libHuff.dylib
CFLAGS = -std=c++11 -Wall -g
GTEST_DIR=/Users/chris.wallerstein/dev/cpp/gtest/googletest/googletest
CC=g++
LIBDIR=src/lib
BUILDDIR=build
TESTDIR=test
LDFLAGS=-Wl,-rpath,/Users/chris.wallerstein/dev/cpp/huff/lib
SRCEXT=cpp
LIB_OBJECTS=$(shell find $(LIBDIR) -type f -name *.$(SRCEXT))
LIB_INCLUDES = $(wildcard src/lib/*.h)
TEST_OBJECTS=$(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC=-Isrc/lib -I/Users/chris.wallerstein/dev/cpp/gtest/googletest/googletest/include/ 
LIB=-L./lib -lHuff

all : lib huff unhuff test
.PHONY : all

lib : $(LIB_OBJECTS) $(LIB_INCLUDES) 
	@echo " Building lib..."
	$(CC) $(LIB_OBJECTS) -shared -dynamiclib -o $(LIB_TARGET) 
.PHONY: lib

huff : lib $(LIB_INCLUDES)
	@echo " Linking huff..."
	$(CC) src/bin/huff_main.cpp -o bin/huff $(LIB) $(INC)
.PHONY: huff

unhuff : lib $(LIB_INCLUDES)
	@echo " Linking unhuff..."
	$(CC) src/bin/unhuff_main.cpp -o bin/unhuff $(LIB) $(INC)
.PHONY: unhuff

clean:
	@echo " Cleaning..."; 
	rm -r bin/* lib/*
.PHONY: clean

test: lib $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o bin/testHuff test/hufftree_test.cpp $(LDFLAGS) $(LIB) \
	$(GTEST_DIR)/libgtest.a $(INC) 

