CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -g

all: test

test: RadixTrieConcurrentSearchTest.cpp
	$(CXX) $(CXXFLAGS) -o RadixTrieConcurrentSearchTest RadixTrieConcurrentSearchTest.cpp
	$(CXX) $(CXXFLAGS) -o RadixTrieTest RadixTrieTest.cpp
	./RadixTrieConcurrentSearchTest
	./RadixTrieTest

clean:
	rm -f RadixTrieTest
