# Makefile for twistec
#
# Authors: bischjer
#          wuurrd
#
# License: GNU PUBLIC LICENSE
#
# -I<dir>                       Also look in <dir> for header files
# -g2                           produce debugging information
# -Wall                         turn on important warnings
# -Werror                       treat warnings as errors
# -O3                           optimize for speed
# -ffast-math                   avoids some checks in math-routines
# -fsingle-precision-constant   use float constants (instead of double)
# -pedantic                     make gcc picky
# -fprofile-arcs                Does profiling in order to optimize branching
# -fbranch-probabilities        Uses the result of profile-arcs to do the actual
#                               branch prediction
#
# Some useful LDFLAGS
#
# -l<lib>                       Link with library
# -L<dir>                       Also look in <dir> for libraries

APP         := bin/example
CXX 		:= g++
LD 			:= g++
BUILDDIR    := _build
DIRS        := $(BUILDDIR) bin lib
SOURCES 	:= $(wildcard *.cpp)
OBJECTS 	:= $(patsubst %.cpp, %.o, $(SOURCES))
CXXFLAGS 	:= $(CXXFLAGS) -pedantic -I .
GTEST_DIR   := lib/googletest
TEST_CXX_FLAGS := -g -Wall -Wextra -I"$(GTEST_DIR)/include"
TEST_LD_FLAGS := -lpthread

LDFLAGS     := $(LDFLAGS)
PWD 		:= $(shell pwd)

TESTS       := reactor_unittest

.PHONY: all clean depend $(TESTS)

all: depend reactor.so $(APP)

reactor.o:
	$(CXX) -o bin/reactor.o $(LDFLAGS) $(CXXFLAGS) -c reactor.cpp

reactor.so: reactor.o
	$(CXX) -shared -o bin/libreactor.so $(LDFLAGS) $(CXXFLAGS) bin/reactor.o

debug: depend $(APP) -g2 -DDEBUG -Wall

depend: gtest_main.a

gtest_main.a:
	cd lib/googletest/make && make && cp gtest_main.a ../../../bin/; cd -;

clean:
	rm -f *.o
	rm -f *~

distclean: clean
	rm -rf $(PWD)/bin/ $(PWD)/static/ $(PWD)/$(BUILDDIR)
	rm make.dep
	rm -f $(APP)

test: $(TESTS)
	./bin/testrunner

reactor_unittest.o: reactor.so
	$(CXX) -I. $(TEST_CXX_FLAGS) -c tests/reactor_unittest.cpp -o bin/reactor_unittest.o

reactor_unittest: reactor_unittest.o
	$(CXX) $(TEST_CXX_FLAGS) $(TEST_LD_FLAGS) -Lbin -lreactor bin/reactor_unittest.o bin/gtest_main.a -o bin/test/reactor_unittest

bin/example: reactor.so
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) example.cpp -Lbin -lreactor -o bin/example