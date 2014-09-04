# jeffsp@gmail.com
# Tue Sep  2 11:16:32 CDT 2014

SHELL=/bin/bash
CCFILES=$(shell ls -1tr *.cc)
TARGETS=$(basename $(CCFILES))

# Dummy targets
BUILD_TARGETS=$(addsuffix .build,$(TARGETS))
RUN_TARGETS=$(addsuffix .run,$(TARGETS))
CLEAN_TARGETS=$(addsuffix .clean,$(TARGETS))

CXX=g++
CXXFLAGS_DEBUG=-std=c++11 -Werror -Wall
CXXFLAGS_RELEASE=-std=c++11 -Werror -Wall -O2 -DNDEBUG
CXXFLAGS=$(CXXFLAGS_RELEASE)

default:
	$(MAKE) -j build

build: $(BUILD_TARGETS)

%.build: %.cc
	$(CXX) $(CXXFLAGS) -o $* $<

run: $(RUN_TARGETS)

%.run: %.cc
	@echo running $*
	./$*

clean: $(CLEAN_TARGETS)

%.clean: %.cc
	@echo removing $$i
	rm -f $*
