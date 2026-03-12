# Name of your executable
TARGET = strato

# Detect OS
UNAME_S := $(shell uname -s)

# Compiler and Basic Flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -g
LIBS = -lncursesw

# Find all .cpp files recursively
SRCS := $(shell find . -name "*.cpp")

# Adjustments for macOS (Homebrew paths)
ifeq ($(UNAME_S), Darwin)
    CXX = clang++
    CXXFLAGS += -I/opt/homebrew/opt/ncurses/include
    LDFLAGS += -L/opt/homebrew/opt/ncurses/lib
endif

# Build Rule
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

# Clean Rule
clean:
	rm -f $(TARGET)

# Build and Run
run: $(TARGET)
	./$(TARGET)
