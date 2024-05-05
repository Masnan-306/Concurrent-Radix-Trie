# Use g++ instead of clang++
CXX = g++

# Compiler and linker flags remain mostly the same; adjust paths if needed
CXXFLAGS = -std=c++17 -Wall -I. -g -fopenmp
LDFLAGS = -L/usr/local/lib
CPPFLAGS = -I/usr/local/include

# Define source files and corresponding object files
SRCS = $(wildcard code/RadixTrie/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# Define test files and corresponding executable names
TEST_SRCS = $(wildcard tests/correctness/*.cpp)
TEST_EXECUTABLES = $(TEST_SRCS:.cpp=)

# Default target, builds all executables
all: $(TEST_EXECUTABLES)

# Rule to build executables from test source files
%: %.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

# Rule to build object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

# Phony target to run all tests
run_tests: $(TEST_EXECUTABLES)
	@for test_exec in $(TEST_EXECUTABLES); do \
		echo "Running $$test_exec..."; \
		./$$test_exec; \
	done

# Clean target
clean:
	rm -f $(OBJS) $(TEST_EXECUTABLES)
	find . -name "*.dSYM" -type d -exec rm -rf {} +

.PHONY: all run_tests clean
