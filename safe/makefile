OUTPUTFILE=huffMain
LIB_FILES=$(wildcard *.cpp)
TEST_FILES=$(wildcard test/*.cpp)
CXXFLAGS = -std=c++98  -pedantic
LIBFLAGS = -dynamiclib
CXX = g++
#INC=-I/Users/chris.wallerstein/boost_1_60_0
#LIB=-L/Users/chris.wallerstein/boost_1_60_0/stage/lib -lboost_thread -lboost_system
INPUT_FILES=$(OUTPUTFILE).cpp bitops.cpp
INSTALLDIR=binaries
LIB_NAME=libHuff.dylib

.PHONY: all
all: $(OUTPUTFILE)

# use implicit rule to build 
#$(OUTPUTFILE) : $(INPUT_FILES)
#	g++ $(CXXFLAGS) $(INC) $(LIB) -o $(OUTPUTFILE) $(INPUT_FILES)

library : $(subst .cpp,.o,$(LIB_FILES))
	$(CXX) -shared $(LIBFLAGS) $(LDFLAGS) -o $(LIB_NAME) $^

test : $(TEST_FILES)
	g++ $(CXXFLAGS) $(INC) $(LIB) -o $(OUTPUTFILE) $(TEST_FILES)

.PHONY: install
install:
	mkdir -p binaries
	cp -p $(OUTPUTFILE) binaries

.PHONY: clean
clean:
	rm -f $(OUTPUTFILE) $(LIB_NAME)

