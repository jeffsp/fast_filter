# jeffsp@gmail.com
# Tue Sep  2 11:16:32 CDT 2014

SHELL=/bin/bash
CCFILES=$(shell ls -1tr *.cc)
TARGETS=$(basename $(CCFILES))

build:
	@for i in $(TARGETS); do \
		echo building $$i.cc...; \
		g++ -std=c++11 -Werror -Wall -o $$i $$i.cc; \
	done

run:
	@for i in $(TARGETS); do \
		echo running $$i; \
		./$$i; \
	done

clean:
	@for i in $(TARGETS); do \
		echo removing $$i; \
		rm -f $$i; \
	done
