# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -Icode/redix  # Include directory for headers

# Directories
CODE_DIR = code/redix
TEST_DIR = tests/correctness

# Source files
SRC = $(CODE_DIR)/RedixTrie.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Target executable
TARGET = $(TEST_DIR)/test

# Default target to compile and link
all: $(TARGET)

# Rule to create object files
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to link everything into an executable
$(TARGET): $(OBJ) $(TEST_DIR)/test.cpp
	$(CXX) $(CXXFLAGS) $(OBJ) $(TEST_DIR)/test.cpp -o $(TARGET)

# Clean up object files and executables
clean:
	rm -f $(OBJ) $(TARGET)
