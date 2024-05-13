CXX=clang++
INCLUDES=-Iincludes/
CXXFLAGS=-std=c++20 -gdwarf-4 -fstandalone-debug -O0
EXTRA_CXX_FLAGS=-Wall -Wextra -Werror -pedantic -Wno-error=unused-parameter

exec: bin/exec
tests: bin/tests

bin/exec: ./src/driver.cc ./includes/illini_book.hpp ./src/illini_book.cc ./includes/utilities.hpp ./src/utilities.cc
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(EXTRA_CXX_FLAGS) ./src/driver.cc ./src/illini_book.cc ./src/utilities.cc -o ./bin/exec

bin/tests : ./includes/illini_book.hpp ./includes/utilities.hpp ./src/illini_book.cc ./src/utilities.cc  ./tests/tests.cc 
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(EXTRA_CXX_FLAGS) ./src/illini_book.cc ./src/utilities.cc ./tests/tests.cc -o bin/tests

.DEFAULT_GOAL := exec
.PHONY: clean exec tests

clean:
	rm -rf bin/* obj/*