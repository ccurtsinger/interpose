CXX := clang++

ifeq ($(shell uname -s),Darwin)
SHLIB_SUFFIX := dylib
PRELOAD_VAR  := DYLD_INSERT_LIBRARIES
CXXLIB       := $(CXX) -shared -fPIC -compatibility_version 1 -current_version 1 \
                        -dynamiclib
else
SHLIB_SUFFIX := so
PRELOAD_VAR  := LD_PRELOAD
CXXLIB       := $(CXX) -shared -fPIC -Wl,-soname,interposer.so
endif

all: logger.$(SHLIB_SUFFIX)

clean:
	@rm -f logger.$(SHLIB_SUFFIX) testprog

logger.$(SHLIB_SUFFIX): logger.cc ../../include/interpose.hh
	$(CXXLIB) -I../../include --std=c++11 -o $@ $< -ldl

testprog: testprog.cc
	$(CXX) --std=c++11 -o $@ $<

test: testprog logger.$(SHLIB_SUFFIX)
	$(PRELOAD_VAR)=./logger.$(SHLIB_SUFFIX) ./testprog

.PHONY: all clean test
