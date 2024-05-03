CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS = -std=c++17 -Wall -I. -g -fopenmp
LDFLAGS = -L/usr/local/opt/llvm/lib
CPPFLAGS = -I/usr/local/opt/llvm/include

# Define all executables
EXECUTABLES = RadixTrieConcurrentSearchTest RadixTrieTest RadixTrieBulkInsertTest

all: $(EXECUTABLES)

RadixTrieConcurrentSearchTest: RadixTrieConcurrentSearchTest.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o RadixTrieConcurrentSearchTest RadixTrieConcurrentSearchTest.cpp

RadixTrieTest: RadixTrieTest.cpp RadixTrieLockFree.tpp RadixTrieLockFineLock.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o RadixTrieTest RadixTrieTest.cpp

RadixTrieBulkInsertTest: RadixTrieBulkInsertTest.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o RadixTrieBulkInsertTest RadixTrieBulkInsertTest.cpp

run_tests: $(EXECUTABLES)
	./RadixTrieConcurrentSearchTest
	./RadixTrieTest
	./RadixTrieBulkInsertTest

clean:
	rm -f $(EXECUTABLES)

.PHONY: all run_tests clean
