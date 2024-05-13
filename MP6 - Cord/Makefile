CXX=clang++
INCLUDES=-Iincludes/
CXXFLAGS=-std=c++20 -gdwarf-4 -fstandalone-debug -O0
EXTRA_CXX_FLAGS=-Wall -Wextra -Werror -pedantic -Wno-error=unused-parameter

exec: bin/exec
tests: bin/tests

bin/exec: ./src/driver.cc ./includes/cord.hpp ./src/cord.cc ./includes/cord_utilities.hpp ./src/cord_utilities.cc ./includes/shared_pointer.hpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(EXTRA_CXX_FLAGS) ./src/driver.cc ./src/cord.cc ./src/cord_utilities.cc -o ./bin/exec

bin/tests : ./includes/shared_pointer.hpp ./includes/cord.hpp ./src/cord.cc ./includes/cord_utilities.hpp ./src/cord_utilities.cc ./tests/tests.cc 
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(EXTRA_CXX_FLAGS) ./src/cord.cc ./src/cord_utilities.cc ./tests/tests.cc -o bin/tests

.DEFAULT_GOAL := exec
.PHONY: clean exec tests

clean:
	rm -rf bin/* obj/*