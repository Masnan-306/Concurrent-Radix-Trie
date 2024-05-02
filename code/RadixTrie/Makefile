CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -g

all: test

test: RadixTrieTest.cpp
	$(CXX) $(CXXFLAGS) -o RadixTrieTest RadixTrieTest.cpp
	./RadixTrieTest

clean:
	rm -f RadixTrieTest
