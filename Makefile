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
DIRS        := $(BUILDDIR) bin static
SOURCES 	:= $(wildcard *.cpp)
OBJECTS 	:= $(patsubst %.cpp, %.o, $(SOURCES))
CXXFLAGS 	:= $(CXXFLAGS) -pedantic
LDFLAGS     := $(LDFLAGS)
PWD 		:= $(shell pwd)

.PHONY: all clean depend

all: depend $(APP)

$(APP): $(OBJECTS)
	$(LD) -o $(APP) $(LDFLAGS) $(OBJECTS)

debug: depend $(APP) -g2 -DDEBUG -Wall

depend: make.dep

make.dep:
	for dir in $(DIRS); do if [ ! -d $$dir ]; then mkdir $$dir; fi; done
	for file in $(SOURCES); do $(CXX) $(CXXFLAGS) -M $$file >> $@; done

include make.dep

dev-install:
	sudo apt-get install git-svn
	git svn clone -r588:HEAD http://googletest.googlecode.com/svn/trunk/ $(PWD)/static/googletest-ro

clean:
	rm -f *.o
	rm -f *~

distclean: clean
	rm -rf $(PWD)/bin/ $(PWD)/static/ $(PWD)/$(BUILDDIR)
	rm make.dep
	rm -f $(APP)