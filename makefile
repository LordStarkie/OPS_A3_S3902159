# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++17

# Target executables
TARGETS = firstfit bestfit

# Source files
SOURCES = mem_manager.cpp main.cpp experiment_helper.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGETS)

firstfit: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

bestfit: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(OBJECTS)

.PHONY: all clean
