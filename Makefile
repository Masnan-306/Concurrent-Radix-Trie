CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS = -std=c++17 -Wall -I. -g -fopenmp
LDFLAGS = -L/usr/local/opt/llvm/lib
CPPFLAGS = -I/usr/local/opt/llvm/include

# Define all executables
EXECUTABLES = RadixTrieConcurrentSearchTest RadixTrieTest RadixTrieBulkInsertTest

all: $(EXECUTABLES)

RadixTrieConcurrentSearchTest: tests/correctness/RadixTrieConcurrentSearchTest.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o RadixTrieConcurrentSearchTest tests/correctness/RadixTrieConcurrentSearchTest.cpp

RadixTrieTest: tests/correctness/RadixTrieTest.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o RadixTrieTest tests/correctness/RadixTrieTest.cpp

RadixTrieBulkInsertTest: tests/correctness/RadixTrieBulkInsertTest.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o RadixTrieBulkInsertTest tests/correctness/RadixTrieBulkInsertTest.cpp

run_tests: $(EXECUTABLES)
	./RadixTrieConcurrentSearchTest
	./RadixTrieTest
	./RadixTrieBulkInsertTest

clean:
	rm -f $(EXECUTABLES)
	find . -name "*.dSYM" -type d -exec rm -rf {} +

.PHONY: all run_tests clean
