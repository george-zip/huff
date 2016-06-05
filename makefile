BIN_TARGET=bin/huffMain
LIB_TARGET=lib/libHuff.dylib
CFLAGS = -std=c++98  -Wall -g
CC=g++
SRCDIR=src
BUILDDIR=build
TESTDIR=test
LDFLAGS=-Wl,-rpath,/Users/chris.wallerstein/dev/cpp/huff/lib
SRCEXT=cpp
SOURCES=$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
TEST_SOURCES=$(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
OBJECTS=$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TEST_OBJECTS=$(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC=-I/Users/chriswallerstein/dev/cpp/gtest/googletest/googletest/include/
LIB=/Users/chriswallerstein/dev/cpp/gtest/googletest/googletest/make/gtest_main.a -L./lib -lHuff
#LIB=-L./lib -lHuff

all : $(LIB_TARGET) $(BIN_TARGET) test
.PHONY : all

$(LIB_TARGET): $(OBJECTS)
	@echo " Building lib..."
	@echo " $(CC) $^ -shared -o $(LIB_TARGET)"; $(CC) $^ -shared -dynamiclib -o $(LIB_TARGET) 

$(BIN_TARGET): $(OBJECTS)
	@echo " Linking binary..."
	@echo " $(CC) $^ -o $(BIN_TARGET) $(LIB)"; $(CC) $^ -o $(BIN_TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(BIN_TARGET) $(LIB_TARGET)"; $(RM) -r $(BUILDDIR) $(BIN_TARGET) $(LIB_TARGET)

# Tests
test: $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o bin/testHuff test/hufftree_test.cpp $(LDFLAGS) $(LIB) $(INC) 

.PHONY: clean
